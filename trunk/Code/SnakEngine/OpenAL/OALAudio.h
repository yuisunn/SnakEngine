#ifndef _OALAudio_H_
#define _OALAudio_H_

#include <SE3DEngine/Config/Config.h>

#ifdef SE_WINDOWS
#include <al.h>
#include <alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

//#include <boost/noncopyable.hpp>
#include <SE3DEngine/Comm/CThread.h>
#include <vector>

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Audio/CAudio.h>
#include <SE3DEngine/Audio/CAudioBuffer.h>
#include <SE3DEngine/Audio/CAudioDataSource.h>
#include <SEMath/Vector3.h>



namespace SE
{
	ALenum Convert(EAudioFormat format);
	Vector3 VecToALVec(Vector3 const & v);
	Vector3 ALVecToVec(Vector3 const & v);
	// 声音缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	class  OALSoundBuffer : public CSoundBuffer
	{
	public:
		OALSoundBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uNumSource, float fVolume);
		~OALSoundBuffer();

		void Play(bool bLoop = false);
		void Stop();

		void SetVolume(float fVol);

		bool IsPlaying() const;

		Vector3 GetPosition() const;
		void SetPosition(Vector3 const & v);
		Vector3 GetVelocity() const;
		void SetVelocity(Vector3 const & v);
		Vector3 GetDirection() const;
		void SetDirection(Vector3 const & v);

	private:
		void DoReset();
		std::vector<ALuint>::iterator FreeSource();

	private:
		std::vector<ALuint>	m_uSources;
		ALuint			m_uBuf;

		Vector3		m_vPos;
		Vector3		m_vVel;
		Vector3		m_vDir;
	};

	// 音乐缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	//boost::noncopyable 的工作原理就是禁止访问它的复制构造函数和赋值操作符，然后使用它作为基类。
	class OALMusicBuffer : public CMusicBuffer
	{
	public:
		OALMusicBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uBufferSeconds, float fVolume);
		~OALMusicBuffer();

		void SetVolume(float fVol);

		bool IsPlaying() const;

		Vector3 GetPosition() const;
		void SetPosition(Vector3 const& cpV);
		Vector3 GetVelocity() const;
		void SetVelocity(Vector3 const&cpV);
		Vector3 GetDirection() const;
		void SetDirection(Vector3 const& cpV);

		void LoopUpdateBuffer();

	private:
		void DoReset();
		void DoPlay(bool bLoop);
		void DoStop();

	private:
		ALuint					m_uSource;
		std::vector<ALuint>		m_uBufQueues;

		bool		m_bLoop;

		bool m_bPlayed;
		bool m_bStopped;
		boost::condition_variable m_PlayCond;
		boost::mutex m_PlayMutex;
		joiner<void> m_PlayThread;
	};

	// 管理音频播放
	/////////////////////////////////////////////////////////////////////////////////
	class OALAudio :  public CAudio
	{
	public:
		OALAudio();
		~OALAudio();

		std::wstring const & Name() const;

		Vector3 GetListenerPos() const;
		void SetListenerPos(Vector3 const & v);
		Vector3 GetListenerVel() const;
		void SetListenerVel(Vector3 const & v);
		void GetListenerOri(Vector3& face, Vector3& up) const;
		void SetListenerOri(Vector3 const & face, Vector3 const & up);

	};
}

#endif