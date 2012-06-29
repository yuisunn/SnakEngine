#ifndef _D3DRenderStateObj_H_
#define _D3DRenderStateObj_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/CRenderStateObj.h>	

namespace SE
{
    class D3D11RasterizerStateObj : public CRasterizerStateObj
	{
	public:
		explicit D3D11RasterizerStateObj(RasterizerStateDesc const & cpDesc);

		void Active();

		ID3D11RasterizerStatePtr const& D3DRasterizerState() const
		{
			return m_pRastState;
		}

	private:
		ID3D11RasterizerStatePtr m_pRastState;
	};

	class D3D11DepthStencilStateObj : public CDepthStencilStateObj
	{
	public:
		explicit D3D11DepthStencilStateObj(DepthStencilStateDesc const& cpDesc);

		void Active(uint16 uFrontStenRef, uint16 uBackStenRef);

		ID3D11DepthStencilStatePtr const & D3DDepthStencilState() const
		{
			return m_pDepthStenState;
		}

	private:
		ID3D11DepthStencilStatePtr m_pDepthStenState;
	};

	class D3D11BlendStateObj : public CBlendStateObj
	{
	public:
		explicit D3D11BlendStateObj(BlendStateDesc const& cpDesc);

		void Active(Color const& cpBlendFactor, uint32 uSampleMask);

		ID3D11BlendStatePtr const & D3DBlendState() const
		{
			return m_pBlendState;
		}

	private:
		ID3D11BlendStatePtr m_pBlendState;
	};

	class D3D11SamplerStateObj : public CSamplerStateObj
	{
	public:
		explicit D3D11SamplerStateObj(SamplerStateDesc const& cpDesc);

		ID3D11SamplerStatePtr const& D3DSamplerState() const
		{
			return m_pSamplerState;
		}

	private:
		ID3D11SamplerStatePtr m_pSamplerState;
	};
}
#endif
