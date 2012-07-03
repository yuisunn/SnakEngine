#ifndef _CRender_H_
#define _CRender_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/Util.h>


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
		void BindFrameBuffer(int const & fb);
		virtual void StartRendering() = 0;

		virtual void BeginFrame() = 0;
		virtual void BeginPass() = 0;
		virtual void EndPass() = 0;
		virtual void EndFrame() = 0;
				// Just for debug or profile propose
		virtual void ForceFlush() = 0;
		virtual bool GetFullScreen() const = 0;
		virtual void SetFullScreen(bool bFull) = 0;

	protected:
		CRasterizerStateObjPtr m_pRasState;
		CBlendStateObjPtr m_pBlendState;
		CDepthStencilStateObjPtr m_pDepthStenState;
		CSamplerStateObjPtr m_pSamplerState;
		uint16 m_uFrontStencilRef;
		uint16 m_uBackStencilRef;
		Color m_vBlendFactor;
	};
 
}

#endif

