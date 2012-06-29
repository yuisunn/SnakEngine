#ifndef _CAudio_H_
#define _CAudio_H_

#include <map>
#include <SE3DEngine/Comm/Def.h>

#include <SE3DEngine/Audio/CAudioDataSource.h>
#include <SEMath/Vector3.h>



namespace SE
{
	//音频缓冲基类
	class DLL_API CAudioBuffer
	{
	public:
		CAudioBuffer(CAudioDataSourcePtr const & cpDataSource);
		virtual ~CAudioBuffer();

		//static CAudioBufferPtr NullObject();

		virtual void Play(bool bLoop = false) = 0;
		virtual void Reset() = 0;
		virtual void Stop() = 0;

		virtual void SetVolume(float fVol) = 0;

		virtual bool IsPlaying() const = 0;
		virtual bool IsSound() const = 0;

		virtual Vector3 GetPosition() const = 0;
		virtual void SetPosition(Vector3 const& cpPos) = 0;
		virtual Vector3 GetVelocity() const = 0;
		virtual void SetVelocity(Vector3 const& cpVel) = 0;
		virtual Vector3 GetDirection() const = 0;
		virtual void SetDirection(Vector3 const& cpDis) = 0;

	protected:
		CAudioDataSourcePtr m_pDataSource;

		EAudioFormat	m_eFormat;
		uint32 m_uFreq;
	};
	// 声音缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	class DLL_API CSoundBuffer : public CAudioBuffer
	{
	public:
		CSoundBuffer(CAudioDataSourcePtr const& cpDataSource);
		virtual ~CSoundBuffer();

		virtual void Reset();

		bool IsSound() const;

	protected:
		virtual void DoReset() = 0;
	};
	// 音乐缓冲区，流式结构
	/////////////////////////////////////////////////////////////////////////////////
	class DLL_API CMusicBuffer : public CAudioBuffer
	{
	public:
		CMusicBuffer(CAudioDataSourcePtr const& cpDataSource);
		virtual ~CMusicBuffer();

		void Play(bool bLoop = false);
		void Stop();
		void Reset();

		bool IsSound() const;

	protected:
		virtual void DoReset() = 0;
		virtual void DoPlay(bool bLoop) = 0;
		virtual void DoStop() = 0;

		// 每秒读取的次数
		static uint32	m_uPreSecond;
	};

	//音频 api封装 隔离
	class DLL_API  CAudio
	{
		typedef std::map<size_t, CAudioBufferPtr> AudioBufs;
		typedef AudioBufs::iterator AudioBufsIter;
		typedef AudioBufs::const_iterator AudioBufsConstIter;
	public:
		CAudio();
		virtual ~CAudio();
		//static CAudioPtr NullObject();

		virtual std::wstring const& GetName() const;

		virtual void AddBuffer(size_t Id, CAudioBufferPtr const& cpBuf);
		size_t NumBuffer() const;
		virtual CAudioBufferPtr GetBuffer(size_t BufId) const;

		void Play(size_t BufID, bool bLoop = false);
		void Stop(size_t BufID);
		void PlayAll(bool bLoop = false);
		void StopAll();
		// 设置和获取音量
		void  SetSoundVolume(float fVol);
		float GetSoundVolume() const;
		void  SetMusicVolume(float fVol);
		float GetMusicVolume() const;

		virtual Vector3 GetListenerPos() const = 0;
		virtual void SetListenerPos(Vector3 const& cpPos) = 0;
		virtual Vector3 GetListenerVel() const = 0;
		virtual void SetListenerVel(Vector3 const& cpV) = 0;
		virtual void GetListenerOri(Vector3& pFace, Vector3& pUp) const = 0;
		virtual void SetListenerOri(Vector3 const& cpFace, Vector3 const& cpUp) = 0;
	protected:
		AudioBufs m_Bufs;
		float m_fSoundVol;
		float m_fMusicVol;

	};

}

#endif
