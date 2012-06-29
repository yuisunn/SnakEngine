//实现不同类型 不同平台插件的加载
#ifndef _SEContext_H_
#define _SEContext_H_

#include <boost/assert.hpp>

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/Util.h>
#include <SE3DEngine/SceneSystem/SceneManager.h>
#include <SE3DEngine/AppLayer/AppFramework.h>
#include <SE3DEngine/AppLayer/RenderConfig.h>

#include <OpenglES/OGLESObjFactory.h>
#include <D3D11/D3D11ObjFactory.h>

namespace SE
{
	struct SConfig
	{
		String m_sAPIObjFacName;
		String m_sAudioFacName;
		String m_sInputFacName;
		//String m_sSceneManagerName;
		//String m_
		SRenderConfig m_RenderCfg;
		bool m_bDeferredRendering;

	};
	class _DLL_API SEContext
	{
	public:
		~SEContext();
		static SEContext& Instance()
		{
			static SEContext context;
			return context;
		}
		CAPIObjFactory& InstFactory()
		{
			//BOOST_ASSERT(m_pFac);
			if(!m_pApiFac)
				m_pApiFac = CreatorObjFactory();
			return *m_pApiFac;
		}
		SceneManager& InsSceneMgr()
		{
			BOOST_ASSERT(m_pSceneMgr);
			return *m_pSceneMgr;
		}

		void InsApp(App3DFramework& pApp)
		{
			m_pApp = pApp;

		}
		bool FactoryIsNull()
		{
		    return m_pFac;
        }
		SConfig& GetCfg()
		{
			return m_Cfg;
		}
		void SetCfg(SConfig const& cpCfg)
		{
			m_Cfg = cpCfg;
		}

		//从文件读取 或xml
		void LoadConfig(SConfig const& cpCfg);



		
	private:
		SEContext();
		CAPIObjFactoryPtr CreatorObjFactory()
		{
			if(m_conf.m_sFacName=="D3D")
			{
			    return MakeSharedPtr<D3D11ObjFactory>();
			}
			else
			{
				return MakeSharedPtr<OGLESObjFactory>();
			}
			
		}
		SceneMgrPtr m_pSceneMgr;
		CAPIObjFactoryPtr m_pApiFac;
		AppFramework* m_pApp; 
		SConfig m_Cfg;
		
	};
}

#endif