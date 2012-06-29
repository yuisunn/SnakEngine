#include <SE3DEngine/Audio/CAudio.h>

#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <SE3DEngine/Comm/Util.h>


namespace SE
{
	// 构造函数
	/////////////////////////////////////////////////////////////////////////////////
	CAudio::CAudio()
					: m_fSoundVol(1), m_fMusicVol(1)
	{
	}

	// 析构函数
	/////////////////////////////////////////////////////////////////////////////////
	CAudio::~CAudio()
	{
	}

	// 往列表里添加一个音频缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::AddBuffer(size_t id, CAudioBufferPtr const& cpBuf)
	{
		m_Bufs.insert(std::make_pair(id, cpBuf));
	}

	// 播放id所指定的声音
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::Play(size_t bufID, bool loop)
	{
		this->GetBuffer(bufID)->Play(loop);
	}

	// 停止id所指定的声音
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::Stop(size_t bufID)
	{
		this->GetBuffer(bufID)->Stop();
	}

	// 播放所有的声音
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::PlayAll(bool bLoop)
	{
		std::for_each(m_Bufs.begin(), m_Bufs.end(),
			boost::bind(&CAudioBuffer::Play,
			//bind（&fun，_1, 1）
			//绑定的参数,fun函数的形参第 1，2 要传入的参数
			//typedef   T   value_type; 容器元素类型 
			//_1  函数掉用的第一个新参绑定到 要绑定的函数 play
			//select2nd 返回AudioBufs 第二个value 的代名词
				boost::bind(select2nd<AudioBufs::value_type>(), _1), bLoop));
	}

	// 停止所有的声音
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::StopAll()
	{
		std::for_each(m_Bufs.begin(), m_Bufs.end(),
			boost::bind(&CAudioBuffer::Stop,
				boost::bind(select2nd<AudioBufs::value_type>(), _1)));
	}

	// 列表里缓冲区的数目
	/////////////////////////////////////////////////////////////////////////////////
	size_t CAudio::NumBuffer() const
	{
		return m_Bufs.size();
	}

	// 获取声音缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	CAudioBufferPtr CAudio::GetBuffer(size_t BufId) const
	{
		AudioBufsConstIter iter(m_Bufs.find(BufId));
		if (iter != m_Bufs.end())
		{
			return iter->second;
		}

		BOOST_ASSERT(false);
		return CAudioBufferPtr();
	}

	// 设置音效音量，vol的取值范围为0--1.0f
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::SetSoundVolume(float vol)
	{
		m_fSoundVol = vol;

		typedef BOOST_TYPEOF(m_Bufs) AudioBufsType;
		BOOST_FOREACH(AudioBufsType::reference ab, m_Bufs)
		{
			if (ab.second->IsSound())
			{
				ab.second->SetVolume(vol);
			}
		}
	}

	// 获取音效音量
	/////////////////////////////////////////////////////////////////////////////////
	float CAudio::GetSoundVolume() const
	{
		return m_fSoundVol;
	}

	// 设置音乐音量，vol的取值范围为0--1.0f
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::SetMusicVolume(float vol)
	{
		m_fMusicVol = vol;

		typedef BOOST_TYPEOF(m_Bufs) AudioBufsType;
		BOOST_FOREACH(AudioBufsType::reference ab, m_Bufs)
		{
			if (!(ab.second->IsSound()))
			{
				ab.second->SetVolume(vol);
			}
		}
	}

	// 获取音乐音量
	/////////////////////////////////////////////////////////////////////////////////
	float CAudio::GetMusicVolume() const
	{
		return m_fMusicVol;
	}
}
