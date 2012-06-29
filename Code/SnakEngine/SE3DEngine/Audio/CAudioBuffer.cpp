#include <SE3DEngine/Audio/CAudio.h>

namespace SE
{
	uint32 CMusicBuffer::m_uPreSecond = 2;
	//Audiobuffer
	CAudioBuffer::CAudioBuffer(CAudioDataSourcePtr const& cpDataSource)
		:m_pDataSource(cpDataSource), m_eFormat(cpDataSource->GetFormat()),m_uFreq(cpDataSource->GetFreq())
	{
	}
	CAudioBuffer::~CAudioBuffer()
    {
	}

	//���� ����
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	CMusicBuffer::CMusicBuffer(CAudioDataSourcePtr const& cpDataSource)
					: CAudioBuffer(cpDataSource)
	{
	}
	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	CMusicBuffer::~CMusicBuffer()
	{
	}

	// �Ƿ�������
	/////////////////////////////////////////////////////////////////////////////////
	bool CMusicBuffer::IsSound() const
	{
		return false;
	}

	// ��������λ�Ա��ڴ�ͷ����
	/////////////////////////////////////////////////////////////////////////////////
	void CMusicBuffer::Reset()
	{
		this->Stop();

		this->DoReset();
	}

	// ������Ƶ��
	/////////////////////////////////////////////////////////////////////////////////
	void CMusicBuffer::Play(bool loop)
	{
		this->DoStop();
		this->DoPlay(loop);
	}

	// ֹͣ������Ƶ��
	////////////////////////////////////////////////////////////////////////////////
	void CMusicBuffer::Stop()
	{
		this->DoStop();
		m_pDataSource->Reset();
	}
	//��������
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	CSoundBuffer::CSoundBuffer(CAudioDataSourcePtr const& cpDataSource)
					: CAudioBuffer(cpDataSource)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	CSoundBuffer::~CSoundBuffer()
	{
	}

	// �Ƿ�����Ч
	/////////////////////////////////////////////////////////////////////////////////
	bool CSoundBuffer::IsSound() const
	{
		return true;
	}

	// ��������λ
	/////////////////////////////////////////////////////////////////////////////////
	void CSoundBuffer::Reset()
	{
		this->Stop();

		m_pDataSource->Reset();

		this->DoReset();
	}


}