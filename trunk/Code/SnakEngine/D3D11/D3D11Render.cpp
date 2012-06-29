#include <SE3DEngine/RenderSystem/CRender.h>
#include <D3D11/D3D11Render.h>
namespace SE
{

	void D3D11Render::BeginFrame()
	{}
	void D3D11Render::BeginPass()
	{}
	void D3D11Render::EndPass()
	{}
	void D3D11Render::EndFrame()
	{}
			// Just for debug or profile propose
	void D3D11Render::ForceFlush()
	{}
	bool D3D11Render::GetFullScreen() const
	{
		return m_bFullSc;
	}
	void D3D11Render::SetFullScreen(bool bFull)
	{
		m_bFullSc = bFull;
	}
	void D3D11Render::SetDevice()
	{}

	D3D_FEATURE_LEVEL D3D11Render::DeviceFeatureLevel() const
	{
		return m_eFeatureLevel;
	}
	//保持 rs cache why
	void D3D11Render::RSSetState(ID3D11RasterizerStatePtr const& cpRas)
	{
		if (m_pRasterizerStateCache != cpRas)
		{
			m_pContext->RSSetState(cpRas.get());
			m_pRasterizerStateCache = cpRas;
		}
	}
	//
	void D3D11Render::OMSetDepthStencilState(ID3D11DepthStencilStatePtr const & cpDeStenc, uint16 uStencilRef)
	{
		if ((m_pDepthStencilStateCache != cpDeStenc) || (m_uStencilRefCache != uStencilRef))
		{
			m_pContext->OMSetDepthStencilState(cpDeStenc.get(), uStencilRef);
			m_pDepthStencilStateCache = cpDeStenc;
			m_uStencilRefCache = uStencilRef;
		}
	}
	//混合状态设置 传入gpu？
	void D3D11Render::OMSetBlendState(ID3D11BlendStatePtr const& cpBlendState, Color const & cpBlendFactor, uint32 uSampleMask)
	{
		if ((m_pBlendStateCache != cpBlendState) || (m_vBlendFactorCache != cpBlendFactor) || (m_uSampleMaskCache != uSampleMask))
		{
			m_pContext->OMSetBlendState(cpBlendState.get(), &cpBlendFactor.x, uSampleMask);
			m_pBlendStateCache = cpBlendState;
			m_vBlendFactorCache = cpBlendFactor;
			m_uSampleMaskCache = uSampleMask;
		}
	}
}

