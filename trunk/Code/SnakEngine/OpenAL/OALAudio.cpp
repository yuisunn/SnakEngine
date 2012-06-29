#include <OpenAL/OALAudio.h>

namespace SE
{
	// ��AudioFormatת��ΪOpenAL�ĸ�ʽ��־
	/////////////////////////////////////////////////////////////////////////////////
	ALenum Convert(EAudioFormat eFormat)
	{
		ALenum out = AL_FORMAT_MONO8;

		switch (eFormat)
		{
		case AF_Mono8:
			out = AL_FORMAT_MONO8;
			break;

		case AF_Mono16:
			out = AL_FORMAT_MONO16;
			break;

		case AF_Stereo8:
			out = AL_FORMAT_STEREO8;
			break;

		case AF_Stereo16:
			out = AL_FORMAT_STEREO16;
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		return out;
	}

	// ����������ϵ��Vector3ת��ΪOpenAL����������ϵ
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 VecToALVec(Vector3 const& cpV)
	{
		return Vector3(cpV.x,cpV.y, -cpV.z);
	}

	// ��OpenAL����������ϵת��Ϊ��������ϵ��Vector3
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 ALVecToVec(Vector3 const& cpV)
	{
		return Vector3(cpV.x,cpV.y, -cpV.z);
	}

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	OALAudio::OALAudio()
	{
		//���� device �� context
		ALCdevice* device(alcOpenDevice(NULL));
		ALCcontext* context(alcCreateContext(device, 0));

		alcMakeContextCurrent(context);

		//���������� λ�� �ٶ� ����
		this->SetListenerPos(Vector3(0, 0, 0.0f));
		this->SetListenerVel(Vector3(0.0f, 0, 0));
		this->SetListenerOri(Vector3(0.0f, 0, 1), Vector3(0.0f, 1, 0));
		//�������������˥���趨	
		alDistanceModel(AL_INVERSE_DISTANCE);

		alDopplerFactor(1);		// ������ʵ�Ķ�����ЧӦ
		alDopplerVelocity(343); //�ٶ� �� ��/�� Ϊ��λ
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	OALAudio::~OALAudio()
	{
		m_Bufs.clear();

		ALCcontext* context(alcGetCurrentContext());
		ALCdevice* device(alcGetContextsDevice(context));

		alcMakeContextCurrent(0);
		//���� ����
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	// ��Ƶ��������
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring const & OALAudio::Name() const
	{
		static std::wstring const name(L"OpenAL Audio Engine");
		return name;
	}

	// ��ȡ3D����λ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALAudio::GetListenerPos() const
	{
		Vector3 v;
		alGetListener3f(AL_POSITION, &v.x, &v.y, &v.z);
		return ALVecToVec(v);
	}

	// ����3D����λ��
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::SetListenerPos(Vector3 const& cpV)
	{
		Vector3 alv(VecToALVec(cpV));
		alListener3f(AL_POSITION, alv.x, alv.y, alv.z);
	}

	// ��ȡ3D�����ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALAudio::GetListenerVel() const
	{
		Vector3 v;
		alGetListener3f(AL_VELOCITY, &v.x, &v.y, &v.z);
		return ALVecToVec(v);
	}

	// ����3D�����ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::SetListenerVel(Vector3 const& cpV)
	{
		Vector3 alv(VecToALVec(cpV));
		alListener3f(AL_VELOCITY, alv.x, alv.y, alv.z);
	}

	// ��ȡ3D���߷���
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::GetListenerOri(Vector3& pFace, Vector3& pUp) const
	{
		float v[6];
		alGetListenerfv(AL_ORIENTATION, v);
		pFace = ALVecToVec(Vector3(v[0],v[1],v[3]));
		pUp = ALVecToVec(Vector3(v[4],v[5],v[6]));
	}

	// ��ȡ3D���߷���
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::SetListenerOri(Vector3 const& cpFace, Vector3 const& cpUp)
	{
		Vector3 alface(VecToALVec(cpFace));
		Vector3 alup(VecToALVec(cpUp));
		float v[6] = { alface.x, alface.y, alface.z, alup.x, alup.y, alup.z };
		alListenerfv(AL_ORIENTATION, v);
	}
}
