#include <OpenAL/OALAudio.h>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/random.hpp>
//music buff
namespace SE
{
	size_t const READSIZE(88200);//不知道什么玩意
		// 检查一个音频缓冲区是否空闲
	/////////////////////////////////////////////////////////////////////////////////
	bool IsSourceFree(ALuint source)
	{
		ALint value;
		alGetSourcei(source, AL_SOURCE_STATE, &value);

		return (AL_PLAYING != (value & AL_PLAYING));
	}
	// 构造函数。建立一个可以用于流式播放的缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	OALMusicBuffer::OALMusicBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uBufferSeconds, float fVolume)
							: CMusicBuffer(cpDataSource),
								m_uBufQueues(uBufferSeconds * m_uPreSecond/*播放速度*/), 
								m_bPlayed(false), m_bStopped(true)
	{
		alGenBuffers(static_cast<ALsizei>(m_uBufQueues.size()), &m_uBufQueues[0]);

		alGenSources(1, &m_uSource);
		alSourcef(m_uSource, AL_PITCH, 1);

		this->SetPosition(Vector3(0, 0, 0.1f));
		this->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
		this->SetDirection(Vector3(0.0f, 0.0f, 0.0f));

		this->SetVolume(fVolume);

		this->Reset();
	}

	// 析构函数
	/////////////////////////////////////////////////////////////////////////////////
	OALMusicBuffer::~OALMusicBuffer()
	{
		this->Stop();

		alDeleteBuffers(static_cast<ALsizei>(m_uBufQueues.size()), &m_uBufQueues[0]);
		alDeleteSources(1, &m_uSource);
	}

	// 更新缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::LoopUpdateBuffer()
	{
		boost::mutex::scoped_lock lock(m_PlayMutex);
		while (!m_bPlayed)
		{
			m_PlayCond.wait(lock);
		}
		m_bPlayed = false;

		while (!m_bStopped)
		{
			ALint processed;
			alGetSourcei(m_uSource, AL_BUFFERS_PROCESSED, &processed);
			if (processed > 0)
			{
				while (processed > 0)
				{
					--processed;

					ALuint buf;
					alSourceUnqueueBuffers(m_uSource, 1, &buf);

					std::vector<uint8_t> data(READSIZE);
					data.resize(m_pDataSource->Read(&data[0], data.size()));
					if (!data.empty())
					{
						alBufferData(buf, Convert(m_Format), &data[0],
							static_cast<ALsizei>(data.size()), m_uFreq);
						alSourceQueueBuffers(m_uSource, 1, &buf);
					}
					else
					{
						if (m_bLoop)
						{
							m_bStopped = false;
							alSourceStopv(1, &m_uSource);
							this->DoReset();
							alSourcePlay(m_uSource);
						}
						else
						{
							m_bStopped = true;
						}
					}
				}
			}
			else
			{
				Sleep(500 / m_uPreSecond);
			}
		}
	}

	// 缓冲区复位以便于从头播放
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::DoReset()
	{
		ALint queued_;
		alGetSourcei(m_uSource, AL_BUFFERS_QUEUED, &queued_);
		if (queued_ > 0)
		{
			std::vector<ALuint> cur_queue(queued_);
			alSourceUnqueueBuffers(m_uSource, queued_, &cur_queue[0]);
		}

		ALenum const format(Convert(m_Format));
		std::vector<uint8_t> data(READSIZE);

		m_pDataSource->Reset();

		ALsizei non_empty_buf = 0;
		// 每个缓冲区中装1 / PreSecond秒的数据
		typedef BOOST_TYPEOF(m_uBufQueues) BufferQueueType;
		BOOST_FOREACH(BufferQueueType::reference buf, m_uBufQueues)
		{
			data.resize(m_pDataSource->Read(&data[0], data.size()));
			if (!data.empty())
			{
				++ non_empty_buf;
				alBufferData(buf, format, &data[0],
					static_cast<ALuint>(data.size()), static_cast<ALuint>(m_uFreq));
			}
			else
			{
				break;
			}
		}

		alSourceQueueBuffers(m_uSource, non_empty_buf, &m_uBufQueues[0]);

		alSourceRewindv(1, &m_uSource);
	}

	// 播放音频流
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::DoPlay(bool loop)
	{
		m_PlayThread = GlobalThreadPool()(boost::bind(&OALMusicBuffer::LoopUpdateBuffer, this));

		m_bLoop = loop;

		m_bStopped = false;
		{
			boost::mutex::scoped_lock lock(m_PlayMutex);
			m_bPlayed = true;
		}
		m_PlayCond.notify_one();

		alSourcei(m_uSource, AL_LOOPING, false);
		alSourcePlay(m_uSource);
	}

	// 停止播放音频流
	////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::DoStop()
	{
		if (!m_bStopped)
		{
			m_bStopped = true;
			m_PlayThread();
		}

		alSourceStopv(1, &m_uSource);
	}

	// 检查缓冲区是否在播放
	/////////////////////////////////////////////////////////////////////////////////
	bool OALMusicBuffer::IsPlaying() const
	{
		ALint value;
		alGetSourcei(m_uSource, AL_SOURCE_STATE, &value);

		return (AL_PLAYING == (value & AL_PLAYING));
	}

	// 设置音量
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetVolume(float vol)
	{
		alSourcef(m_uSource, AL_GAIN, vol);
	}

	// 获取声源位置
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::GetPosition() const
	{
		float pos[3];
		alGetSourcefv(m_uSource, AL_POSITION, pos);
		return ALVecToVec(Vector3(pos[0], pos[1], pos[2]));
	}

	// 设置声源位置
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetPosition(Vector3 const & v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(m_uSource, AL_POSITION, &alv.x);
	}

	// 获取声源速度
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::GetVelocity() const
	{
		float vel[3];
		alGetSourcefv(m_uSource, AL_VELOCITY, vel);
		return ALVecToVec(Vector3(vel[0], vel[1], vel[2]));
	}

	// 设置声源速度
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetVelocity(Vector3 const & v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(m_uSource, AL_VELOCITY, &alv.x);
	}

	// 获取声源方向
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::GetDirection() const
	{
		float dir[3];
		alGetSourcefv(m_uSource, AL_DIRECTION, dir);
		return ALVecToVec(Vector3(dir[0], dir[1], dir[2]));
	}

	// 设置声源方向
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetDirection(Vector3 const & v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(m_uSource, AL_DIRECTION, &alv.x);
	}

	//sound  buffer
	// 构造函数
	/////////////////////////////////////////////////////////////////////////////////
	OALSoundBuffer::OALSoundBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uNumSource, float fVolume)
						: CSoundBuffer(cpDataSource),
							m_uSources(uNumSource)
	{
		alGenBuffers(1, &m_uBuf);

		// 用整个waveFile填充缓冲区
		std::vector<uint8_t> data(m_pDataSource->GetSize());
		m_pDataSource->Read(&data[0], data.size());

		alBufferData(m_uBuf, Convert(m_Format), &data[0], static_cast<ALsizei>(data.size()), m_uFreq);

		alGenSources(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);

		typedef BOOST_TYPEOF(m_uSources) SourcesType;
		BOOST_FOREACH(SourcesType::reference source, m_uSources)
		{
			alSourcef(source, AL_PITCH, 1);
			alSourcef(source, AL_GAIN, fVolume);
			alSourcei(source, AL_BUFFER, m_uBuf);
		}

		this->SetPosition(Vector3(0, 0, 0.1f));
		this->SetVelocity(Vector3(0.0f, 0, 0));
		this->SetDirection(Vector3(0.0f, 0, 0));

		this->Reset();
	}

	// 析构函数
	/////////////////////////////////////////////////////////////////////////////////
	OALSoundBuffer::~OALSoundBuffer()
	{
		this->Stop();

		alDeleteBuffers(1, &m_uBuf);
		alDeleteSources(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);
	}

	// 返回空闲的缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	std::vector<ALuint>::iterator OALSoundBuffer::FreeSource()
	{
		BOOST_ASSERT(!m_uSources.empty());

		BOOST_AUTO(iter, std::find_if(m_uSources.begin(), m_uSources.end(), IsSourceFree));
		if (iter == m_uSources.end())
		{
			iter = m_uSources.begin();
			std::advance(iter,
				boost::variate_generator<boost::lagged_fibonacci607, boost::uniform_int<> >(boost::lagged_fibonacci607(),
					boost::uniform_int<>(0, static_cast<int>(m_uSources.size())))());
		}

		return iter;
	}

	// 播放音源
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::Play(bool bLoop)
	{
		ALuint& source(*this->FreeSource());

		alSourcefv(source, AL_POSITION, &m_vPos[0]);
		alSourcefv(source, AL_VELOCITY, &m_vVel[0]);
		alSourcefv(source, AL_DIRECTION, &m_vDir[0]);
		alSourcei(source, AL_LOOPING, bLoop);

		alSourcePlay(source);
	}

	// 停止播放
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::Stop()
	{
		alSourceStopv(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);
	}

	// 声音缓冲区复位
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::DoReset()
	{
		alSourceRewindv(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);
	}

	// 检查缓冲区是否在播放
	/////////////////////////////////////////////////////////////////////////////////
	bool OALSoundBuffer::IsPlaying() const
	{
		return (std::find_if(m_uSources.begin(), m_uSources.end(),
			!boost::bind(IsSourceFree, _1)) != m_uSources.end());
	}

	// 设置音量
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetVolume(float vol)
	{
		std::for_each(m_uSources.begin(), m_uSources.end(),
			boost::bind(alSourcef, _1, AL_GAIN, vol));
	}

	// 获取声源位置
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALSoundBuffer::GetPosition() const
	{
		return ALVecToVec(m_vPos);
	}

	// 设置声源位置
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetPosition(Vector3 const & v)
	{
		m_vPos = VecToALVec(v);
	}

	// 获取声源速度
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALSoundBuffer::GetVelocity() const
	{
		return ALVecToVec(m_vVel);
	}

	// 设置声源速度
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetVelocity(Vector3 const & v)
	{
		m_vVel = VecToALVec(v);
	}

	// 获取声源方向
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALSoundBuffer::GetDirection() const
	{
		return ALVecToVec(m_vDir);
	}

	// 设置声源方向
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetDirection(Vector3 const & v)
	{
		m_vDir = VecToALVec(v);
	}
}

