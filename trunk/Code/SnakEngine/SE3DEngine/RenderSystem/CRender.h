#ifndef _CRender_H_
#define _CRender_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/Util.h>
#include <SE3DEngine/AppLayer/RenderConfig.h>
#include <SE3DEngine/RenderSystem/DeviceCaps.h>

namespace SE
{
	class DLL_API CRender
	{
	public:
		CRender();
		virtual ~CRender();

	   //¹âÕ¤»¯×´Ì¬
		CRasterizerStateObjPtr const& CurRasStateObj() const
		{
			return m_pRasState;
		}
		CBlendStateObjPtr const& CurBlendStateObj() const
		{
			return m_pBlendState;
		}
		Color const& CurBlendFactor() const
		{
			return m_vBlendFactor;
		}
		CDepthStencilStateObjPtr const& CurDepthStenStateObj() const
		{
			return m_pDepthStenState;
		}
		uint16 CurFrontStencilRef() const
		{
			return m_uFrontStencilRef;
		}
		uint16 CurBackStencilRef() const
		{
			return m_uBackStencilRef;
		}
		SDeviceCaps const& GetDeviceCaps() const;
		EStereoMethod GetStereo() const
		{
			return m_eStereoMethod;
		}
		void SetStereo(EStereoMethod eMethod);
		

	
		void BindFrameBuffer(CFrameBufferPtr const& cpFb);
		void  SetStateObjs(CRasterizerStateObjPtr const& cpRSobj,
		CDepthStencilStateObjPtr const& cpDSSobj, uint16 uFrontStencilRef, uint16 uBackStencilRef,
		CBlendStateObjPtr const& cpBSobj, Color const& cpBlendFactor, uint32 uSampleMask);
		CFrameBufferPtr const& CurFrameBuffer() const;
		CFrameBufferPtr const& DefaultFrameBuffer() const;
		CFrameBufferPtr const& ScreenFrameBuffer() const;

		void CreateRenderWindow(std::string const& cpName, SRenderConfig const & cpSettings);
		// Scissor support
		virtual void ScissorRect(uint32 uX, uint32 uY, uint32 uWidth, uint32 uHeight) = 0;
	protected:
		virtual void StartRendering() = 0;

		virtual void BeginFrame() = 0;
		virtual void BeginPass() = 0;
		virtual void EndPass() = 0;
		virtual void EndFrame() = 0;
				// Just for debug or profile propose
		virtual void ForceFlush() = 0;
		virtual bool GetFullScreen() const = 0;
		virtual void SetFullScreen(bool bFull) = 0;
		virtual void DoCreateRenderWindow(std::string const & name, SRenderConfig const & settings) = 0;
		virtual void DoBindFrameBuffer(CFrameBufferPtr const & fb) = 0;
		//virtual void DoBindSOBuffers(RenderLayoutPtr const & rl) = 0;
		//virtual void DoRender(RenderTechnique const & tech, RenderLayout const & rl) = 0;
		//virtual void DoDispatch(RenderTechnique const & tech, uint32_t tgx, uint32_t tgy, uint32_t tgz) = 0;
		//virtual void DoResize(uint32_t width, uint32_t height) = 0;
	private:
		virtual void CheckConfig();
	protected:

		CRasterizerStateObjPtr m_pRasState;
		CBlendStateObjPtr m_pBlendState;
		CDepthStencilStateObjPtr m_pDepthStenState;
		CSamplerStateObjPtr m_pSamplerState;
		uint16 m_uFrontStencilRef;
		uint16 m_uBackStencilRef;
		Color m_vBlendFactor;
		uint32 m_SampleMask;

		SRenderConfig m_cfg;
		EStereoMethod m_eStereoMethod;
        CFrameBufferPtr m_ScreenFrameBuffer;
		CFrameBufferPtr m_CurFrameBuffer;
		CFrameBufferPtr m_DefaultFrameBuffers[5];

        bool m_bHdr;
		bool m_bPPAA;
		bool m_ColorGrading;

		float m_fStereoSeparation;
	};
 
}

#endif

