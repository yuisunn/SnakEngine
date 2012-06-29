#include <OpenAL/OALAudio.h>

namespace SE
{
	// 从AudioFormat转化为OpenAL的格式标志
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

	// 从左手坐标系的Vector3转化为OpenAL的右手坐标系
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 VecToALVec(Vector3 const& cpV)
	{
		return Vector3(cpV.x,cpV.y, -cpV.z);
	}

	// 从OpenAL的右手坐标系转化为左手坐标系的Vector3
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 ALVecToVec(Vector3 const& cpV)
	{
		return Vector3(cpV.x,cpV.y, -cpV.z);
	}

	// 构造函数
	/////////////////////////////////////////////////////////////////////////////////
	OALAudio::OALAudio()
	{
		//创建 device 和 context
		ALCdevice* device(alcOpenDevice(NULL));
		ALCcontext* context(alcCreateContext(device, 0));

		alcMakeContextCurrent(context);

		//设置收听者 位置 速度 方向
		this->SetListenerPos(Vector3(0, 0, 0.0f));
		this->SetListenerVel(Vector3(0.0f, 0, 0));
		this->SetListenerOri(Vector3(0.0f, 0, 1), Vector3(0.0f, 1, 0));
		//距离产生的声音衰竭设定	
		alDistanceModel(AL_INVERSE_DISTANCE);

		alDopplerFactor(1);		// 按照现实的多普勒效应
		alDopplerVelocity(343); //速度 以 米/秒 为单位
	}

	// 析构函数
	/////////////////////////////////////////////////////////////////////////////////
	OALAudio::~OALAudio()
	{
		m_Bufs.clear();

		ALCcontext* context(alcGetCurrentContext());
		ALCdevice* device(alcGetContextsDevice(context));

		alcMakeContextCurrent(0);
		//销毁 对象
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	// 音频引擎名字
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring const & OALAudio::Name() const
	{
		static std::wstring const name(L"OpenAL Audio Engine");
		return name;
	}

	// 获取3D听者位置
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALAudio::GetListenerPos() const
	{
		Vector3 v;
		alGetListener3f(AL_POSITION, &v.x, &v.y, &v.z);
		return ALVecToVec(v);
	}

	// 设置3D听者位置
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::SetListenerPos(Vector3 const& cpV)
	{
		Vector3 alv(VecToALVec(cpV));
		alListener3f(AL_POSITION, alv.x, alv.y, alv.z);
	}

	// 获取3D听者速度
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALAudio::GetListenerVel() const
	{
		Vector3 v;
		alGetListener3f(AL_VELOCITY, &v.x, &v.y, &v.z);
		return ALVecToVec(v);
	}

	// 设置3D听者速度
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::SetListenerVel(Vector3 const& cpV)
	{
		Vector3 alv(VecToALVec(cpV));
		alListener3f(AL_VELOCITY, alv.x, alv.y, alv.z);
	}

	// 获取3D听者方向
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::GetListenerOri(Vector3& pFace, Vector3& pUp) const
	{
		float v[6];
		alGetListenerfv(AL_ORIENTATION, v);
		pFace = ALVecToVec(Vector3(v[0],v[1],v[3]));
		pUp = ALVecToVec(Vector3(v[4],v[5],v[6]));
	}

	// 获取3D听者方向
	/////////////////////////////////////////////////////////////////////////////////
	void OALAudio::SetListenerOri(Vector3 const& cpFace, Vector3 const& cpUp)
	{
		Vector3 alface(VecToALVec(cpFace));
		Vector3 alup(VecToALVec(cpUp));
		float v[6] = { alface.x, alface.y, alface.z, alup.x, alup.y, alup.z };
		alListenerfv(AL_ORIENTATION, v);
	}
}
