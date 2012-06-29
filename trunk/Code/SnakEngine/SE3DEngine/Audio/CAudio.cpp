#include <SE3DEngine/Audio/CAudio.h>

#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <SE3DEngine/Comm/Util.h>


namespace SE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	CAudio::CAudio()
					: m_fSoundVol(1), m_fMusicVol(1)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	CAudio::~CAudio()
	{
	}

	// ���б������һ����Ƶ������
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::AddBuffer(size_t id, CAudioBufferPtr const& cpBuf)
	{
		m_Bufs.insert(std::make_pair(id, cpBuf));
	}

	// ����id��ָ��������
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::Play(size_t bufID, bool loop)
	{
		this->GetBuffer(bufID)->Play(loop);
	}

	// ֹͣid��ָ��������
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::Stop(size_t bufID)
	{
		this->GetBuffer(bufID)->Stop();
	}

	// �������е�����
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::PlayAll(bool bLoop)
	{
		std::for_each(m_Bufs.begin(), m_Bufs.end(),
			boost::bind(&CAudioBuffer::Play,
			//bind��&fun��_1, 1��
			//�󶨵Ĳ���,fun�������βε� 1��2 Ҫ����Ĳ���
			//typedef   T   value_type; ����Ԫ������ 
			//_1  �������õĵ�һ���²ΰ󶨵� Ҫ�󶨵ĺ��� play
			//select2nd ����AudioBufs �ڶ���value �Ĵ�����
				boost::bind(select2nd<AudioBufs::value_type>(), _1), bLoop));
	}

	// ֹͣ���е�����
	/////////////////////////////////////////////////////////////////////////////////
	void CAudio::StopAll()
	{
		std::for_each(m_Bufs.begin(), m_Bufs.end(),
			boost::bind(&CAudioBuffer::Stop,
				boost::bind(select2nd<AudioBufs::value_type>(), _1)));
	}

	// �б��ﻺ��������Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t CAudio::NumBuffer() const
	{
		return m_Bufs.size();
	}

	// ��ȡ����������
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

	// ������Ч������vol��ȡֵ��ΧΪ0--1.0f
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

	// ��ȡ��Ч����
	/////////////////////////////////////////////////////////////////////////////////
	float CAudio::GetSoundVolume() const
	{
		return m_fSoundVol;
	}

	// ��������������vol��ȡֵ��ΧΪ0--1.0f
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

	// ��ȡ��������
	/////////////////////////////////////////////////////////////////////////////////
	float CAudio::GetMusicVolume() const
	{
		return m_fMusicVol;
	}
}
