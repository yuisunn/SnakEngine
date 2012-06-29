#include <OpenAL/OALAudio.h>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/random.hpp>
//music buff
namespace SE
{
	size_t const READSIZE(88200);//��֪��ʲô����
		// ���һ����Ƶ�������Ƿ����
	/////////////////////////////////////////////////////////////////////////////////
	bool IsSourceFree(ALuint source)
	{
		ALint value;
		alGetSourcei(source, AL_SOURCE_STATE, &value);

		return (AL_PLAYING != (value & AL_PLAYING));
	}
	// ���캯��������һ������������ʽ���ŵĻ�����
	/////////////////////////////////////////////////////////////////////////////////
	OALMusicBuffer::OALMusicBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uBufferSeconds, float fVolume)
							: CMusicBuffer(cpDataSource),
								m_uBufQueues(uBufferSeconds * m_uPreSecond/*�����ٶ�*/), 
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

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	OALMusicBuffer::~OALMusicBuffer()
	{
		this->Stop();

		alDeleteBuffers(static_cast<ALsizei>(m_uBufQueues.size()), &m_uBufQueues[0]);
		alDeleteSources(1, &m_uSource);
	}

	// ���»�����
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

	// ��������λ�Ա��ڴ�ͷ����
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
		// ÿ����������װ1 / PreSecond�������
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

	// ������Ƶ��
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

	// ֹͣ������Ƶ��
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

	// ��黺�����Ƿ��ڲ���
	/////////////////////////////////////////////////////////////////////////////////
	bool OALMusicBuffer::IsPlaying() const
	{
		ALint value;
		alGetSourcei(m_uSource, AL_SOURCE_STATE, &value);

		return (AL_PLAYING == (value & AL_PLAYING));
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetVolume(float vol)
	{
		alSourcef(m_uSource, AL_GAIN, vol);
	}

	// ��ȡ��Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::GetPosition() const
	{
		float pos[3];
		alGetSourcefv(m_uSource, AL_POSITION, pos);
		return ALVecToVec(Vector3(pos[0], pos[1], pos[2]));
	}

	// ������Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetPosition(Vector3 const & v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(m_uSource, AL_POSITION, &alv.x);
	}

	// ��ȡ��Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::GetVelocity() const
	{
		float vel[3];
		alGetSourcefv(m_uSource, AL_VELOCITY, vel);
		return ALVecToVec(Vector3(vel[0], vel[1], vel[2]));
	}

	// ������Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetVelocity(Vector3 const & v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(m_uSource, AL_VELOCITY, &alv.x);
	}

	// ��ȡ��Դ����
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::GetDirection() const
	{
		float dir[3];
		alGetSourcefv(m_uSource, AL_DIRECTION, dir);
		return ALVecToVec(Vector3(dir[0], dir[1], dir[2]));
	}

	// ������Դ����
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::SetDirection(Vector3 const & v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(m_uSource, AL_DIRECTION, &alv.x);
	}

	//sound  buffer
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	OALSoundBuffer::OALSoundBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uNumSource, float fVolume)
						: CSoundBuffer(cpDataSource),
							m_uSources(uNumSource)
	{
		alGenBuffers(1, &m_uBuf);

		// ������waveFile��仺����
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

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	OALSoundBuffer::~OALSoundBuffer()
	{
		this->Stop();

		alDeleteBuffers(1, &m_uBuf);
		alDeleteSources(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);
	}

	// ���ؿ��еĻ�����
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

	// ������Դ
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

	// ֹͣ����
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::Stop()
	{
		alSourceStopv(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);
	}

	// ������������λ
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::DoReset()
	{
		alSourceRewindv(static_cast<ALsizei>(m_uSources.size()), &m_uSources[0]);
	}

	// ��黺�����Ƿ��ڲ���
	/////////////////////////////////////////////////////////////////////////////////
	bool OALSoundBuffer::IsPlaying() const
	{
		return (std::find_if(m_uSources.begin(), m_uSources.end(),
			!boost::bind(IsSourceFree, _1)) != m_uSources.end());
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetVolume(float vol)
	{
		std::for_each(m_uSources.begin(), m_uSources.end(),
			boost::bind(alSourcef, _1, AL_GAIN, vol));
	}

	// ��ȡ��Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALSoundBuffer::GetPosition() const
	{
		return ALVecToVec(m_vPos);
	}

	// ������Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetPosition(Vector3 const & v)
	{
		m_vPos = VecToALVec(v);
	}

	// ��ȡ��Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALSoundBuffer::GetVelocity() const
	{
		return ALVecToVec(m_vVel);
	}

	// ������Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetVelocity(Vector3 const & v)
	{
		m_vVel = VecToALVec(v);
	}

	// ��ȡ��Դ����
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALSoundBuffer::GetDirection() const
	{
		return ALVecToVec(m_vDir);
	}

	// ������Դ����
	/////////////////////////////////////////////////////////////////////////////////
	void OALSoundBuffer::SetDirection(Vector3 const & v)
	{
		m_vDir = VecToALVec(v);
	}
}

