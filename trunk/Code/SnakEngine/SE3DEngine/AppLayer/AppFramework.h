#ifndef _AppFramework_H_
#define _AppFramework_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Config/Platform.h>
#include <SEMath/Vector3.h>
#include <SE3DEngine/Comm/Time.h>

#include <SE3DEngine/AppLayer/RenderConfig.h>


namespace SE
{
	//吧 sence audio 脚本等 集中初始化
	// 一个用于创建3D应用程序框架的基类。建立一个3D应用程序需要继承这个类，
	//			然后重载以下函数:
	//
	//			InitObjects()			- 初始化3D设备
	//			Update()				- 刷新场景
	//			DelObjects()			- 清除3D场景
	class  DLL_API AppFramework
	{
		friend class SceneManager;

	public:
		enum UpdateRetVal
		{
			URV_Need_Flush = 1UL << 0,
			URV_Flushed = 1UL << 1,
			URV_Finished = 1UL << 2,
			URV_Overlay = 1UL << 3,
			URV_Skip_Postprocess = 1UL << 4,
			URV_Opaque_Only = 1UL << 5,
			URV_Transparency_Only = 1UL << 6,
			URV_Special_Shading_Only = 1UL << 7
		};

	public:
		explicit AppFramework(String const& sName);
		virtual ~AppFramework();

		virtual void Create();
		virtual void Destroy();

		WindowPtr MakeWindow(String const& sName, SRenderConfig const& cpConfig);
		WindowPtr MainWnd() const
		{
			return m_pWind;
		}

#ifdef SE_ANDROID
		android_app* AppState() const
		{
			return m_pState;
		}
#endif

		virtual bool ConfirmDevice() const
		{
			return true;
		}

		Camera const & ActiveCamera() const;
		Camera& ActiveCamera();

		float FPS() const;

		void Run();
		void Quit();

		virtual void OnResize(uint32 uWidth, uint32 uHeight);

	protected:
		void LookAt(Vector3 const& vEye, Vector3 const& vLookAt);
		void LookAt(Vector3 const& vEye, Vector3 const& vLookAt, Vector3 const& vUp);
		void Proj(float fNearPlane, float fFarPlane);

	protected:
		virtual void InitObjects()
		{
		}

		uint32 Update(uint32 uPass);
		void UpdateStats();

		virtual void RenderOver()
		{
		}

		virtual void DelObjects()
		{
		}

	private:
		virtual void DoUpdateOverlay() = 0;
		virtual uint32 DoUpdate(uint32 uPass) = 0;

	protected:
		String m_sName;
#ifdef SE_ANDROID
		android_app* m_pState;
#endif

		// Stats
		float	m_fFPS;
		float	m_fFrameTime;
		float	m_fAccumulateTime;
		uint32 m_uNumFrames;

		Timer m_Timer;

		WindowPtr m_pWind;
	};
}

#endif