

#include <boost/assert.hpp>
#include <SE3DEngine/Comm/MonFac.h>

#include <SE3DEngine/AppLayer/AppFramework.h>
namespace SE
{
	// 构造函数
	/////////////////////////////////////////////////////////////////////////////////
	AppFramework::AppFramework(String const& sName)
						: m_sName(sName),
							m_fFPS(0), m_fAccumulateTime(0), m_uNumFrames(0)
	{
		MonFac::Only().InstApp(*this);

#ifdef SE_ANDROID
		m_pState = get_app();
#endif

		SConfig cfg = MonFac::Only().GetCfg();
		m_pWind = this->MakeWindow(sName, cfg.m_RenderCfg);
		cfg.m_RenderCfg.iLeft = m_pWind->GetLeft();
		cfg.m_RenderCfg.iTop = m_pWind->GetTop();
		cfg.m_RenderCfg.iWidth = m_pWind->GetWidth();
		cfg.m_RenderCfg.iHeight = m_pWind->GetHeight();
		MonFac::Only().Config(cfg);
	}

	AppFramework::~AppFramework()
	{
		this->Destroy();

		m_pWind.reset();
	}

	// 建立应用程序主窗口
	/////////////////////////////////////////////////////////////////////////////////
	void AppFramework::Create()
	{
		SConfig const & cfg =  MonFac::Only().GetCfg();
		MonFac::Only().FacR().c ().RenderEngineInstance().CreateRenderWindow(name_,
			cfg.graphics_cfg);

		//if (cfg.deferred_rendering)
		//{
		//	Context::Instance().DeferredRenderingLayerInstance(MakeSharedPtr<DeferredRenderingLayer>());
		//}

		this->InitObjects();
		this->OnResize(cfg.graphics_cfg.width, cfg.graphics_cfg.height);
	}

	void AppFramework::Destroy()
	{
		this->DelObjects();

		Context::Instance().SceneManagerInstance(SceneManagerPtr());
		Context::Instance().AudioFactoryInstance(AudioFactoryPtr());
		Context::Instance().ShowFactoryInstance(ShowFactoryPtr());

		UIManager::ForceDestroy();
		Context::Instance().DeferredRenderingLayerInstance(DeferredRenderingLayerPtr());
		Context::Instance().RenderFactoryInstance(RenderFactoryPtr());
	}

	WindowPtr AppFramework::MakeWindow(std::string const & name, RenderSettings const & settings)
	{
		return MakeSharedPtr<Window>(name, settings);
	}

	void AppFramework::Run()
	{
		Context::Instance().RenderFactoryInstance().RenderEngineInstance().StartRendering();

		this->DelObjects();
	}

	// 获取当前摄像机
	/////////////////////////////////////////////////////////////////////////////////
	Camera const & AppFramework::ActiveCamera() const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		CameraPtr const & camera = re.CurFrameBuffer()->GetViewport().camera;
		BOOST_ASSERT(camera);

		return *camera;
	}

	Camera& AppFramework::ActiveCamera()
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		CameraPtr const & camera = re.CurFrameBuffer()->GetViewport().camera;
		BOOST_ASSERT(camera);

		return *camera;
	}

	// 设置观察矩阵
	/////////////////////////////////////////////////////////////////////////////////
	void AppFramework::LookAt(float3 const & vEye, float3 const & vLookAt)
	{
		this->ActiveCamera().ViewParams(vEye, vLookAt, float3(0, 1, 0));
	}

	void AppFramework::LookAt(float3 const & vEye, float3 const & vLookAt,
												float3 const & vUp)
	{
		this->ActiveCamera().ViewParams(vEye, vLookAt, vUp);
	}

	// 设置投射矩阵
	/////////////////////////////////////////////////////////////////////////////////
	void AppFramework::Proj(float nearPlane, float farPlane)
	{
		BOOST_ASSERT(nearPlane != 0);
		BOOST_ASSERT(farPlane != 0);
		BOOST_ASSERT(farPlane > nearPlane);

		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		FrameBuffer& fb = *re.CurFrameBuffer();

		this->ActiveCamera().ProjParams(PI / 4, static_cast<float>(fb.Width()) / fb.Height(),
			nearPlane, farPlane);
 	}

	// 退出程序
	/////////////////////////////////////////////////////////////////////////////////
	void AppFramework::Quit()
	{
#ifdef KLAYGE_PLATFORM_WINDOWS
		::PostQuitMessage(0);
#else
		exit(0);
#endif
	}

	// 更新场景
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t AppFramework::Update(uint32_t pass)
	{
		if (0 == pass)
		{
			this->UpdateStats();
			this->DoUpdateOverlay();
		}

		return this->DoUpdate(pass);
	}

	// 响应窗口大小变化
	/////////////////////////////////////////////////////////////////////////////////
	void AppFramework::OnResize(uint32_t /*width*/, uint32_t /*height*/)
	{
		this->Proj(this->ActiveCamera().NearPlane(), this->ActiveCamera().FarPlane());
	}

	// 更新状态
	/////////////////////////////////////////////////////////////////////////////////
	void AppFramework::UpdateStats()
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		if (re.StereoActiveEye() != 0)
		{
			return;
		}

		// measure statistics
		++ num_frames_;
		accumulate_time_ += static_cast<float>(timer_.elapsed());

		// check if new second
		if (accumulate_time_ > 1)
		{
			// new second - not 100% precise
			fps_ = num_frames_ / accumulate_time_;

			accumulate_time_ = 0;
			num_frames_  = 0;
		}

		timer_.restart();
	}

	// 获取渲染目标的每秒帧数
	/////////////////////////////////////////////////////////////////////////////////
	float AppFramework::FPS() const
	{
		return fps_;
	}
}
