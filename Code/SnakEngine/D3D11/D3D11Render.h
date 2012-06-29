//d3d 函数封装 不直接接触的
#ifndef _D3D11Render_H_
#define _D3D11Render_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/CRender.h>


namespace SE
{
	class _DLL_API D3D11Render :public CRender
	{
	public:
		D3D11Render();
		~D3D11Render();
		ID3D11DeviceContextPtr const& GetD3DDeviceImmContext() const;
		ID3D11DevicePtr const& GetD3DDevice() const;
		D3D_FEATURE_LEVEL DeviceFeatureLevel() const;
		void RSSetState(ID3D11RasterizerStatePtr const& cpRas);
		void OMSetDepthStencilState(ID3D11DepthStencilStatePtr const & cpDeStenc, uint16 uStencilRef);
		void OMSetBlendState(ID3D11BlendStatePtr const& cpBlendState, Color const & cpBlendFactor, uint32 uSampleMask);
		void StartRendering();
		void BeginFrame();
		void BeginPass();
		void EndPass();
		void EndFrame();
				// Just for debug or profile propose
		void ForceFlush();
		bool GetFullScreen() const;
		void SetFullScreen(bool bFull);
		void SetDevice();
	private:
		bool m_bFullSc;
		ID3D11DevicePtr		m_pDevice;
		ID3D11DeviceContextPtr m_pContext;
		//d3d 版本支持
		D3D_FEATURE_LEVEL m_eFeatureLevel;
		//state
		ID3D11RasterizerStatePtr m_pRasterizerStateCache;
		ID3D11DepthStencilStatePtr m_pDepthStencilStateCache;
		uint16 m_uStencilRefCache;
		ID3D11BlendStatePtr m_pBlendStateCache;
		Color m_vBlendFactorCache;
		uint32 m_uSampleMaskCache;
	};
}

#endif
