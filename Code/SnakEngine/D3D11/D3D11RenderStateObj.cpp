
#include <D3D11/D3D11RenderStateObj.h>

#include <limits>

#include <SE3DEngine/Comm/ThrowErr.h>
#include <SE3DEngine/Comm/Util.h>
#include <D3D11/D3D11Mapping.h>
#include <D3D11/D3D11Render.h>
#include <SE3DEngine/Comm/SEContext.h>


namespace SE
{
	////光栅化状态初始化
	//D3D11RasterizerStateObj::D3D11RasterizerStateObj(RasterizerStateDesc const& cpDesc)
	//	: CRasterizerStateObj(cpDesc)
	//{
	//	D3D11_RASTERIZER_DESC d3d_desc;
	//	//裁剪 和遮挡 statesave
	//	d3d_desc.FillMode = D3D11Mapping::Mapping(cpDesc.m_ePolygonMode);
	//	d3d_desc.CullMode = D3D11Mapping::Mapping(cpDesc.m_eCullMode);
	//	d3d_desc.FrontCounterClockwise = D3D11Mapping::Mapping(cpDesc.m_bFrontFaceCCW);
	//	d3d_desc.DepthBias = static_cast<int>(cpDesc.m_fPolygonOffsetUnits);
	//	d3d_desc.DepthBiasClamp = cpDesc.m_fPolygonOffsetUnits;
	//	d3d_desc.SlopeScaledDepthBias = cpDesc.m_fPolygonOffsetFactor;
	//	d3d_desc.DepthClipEnable = cpDesc.m_bDepthClipEnable;
	//	//裁剪
	//	d3d_desc.ScissorEnable = cpDesc.m_bScissorEnable;
	//	d3d_desc.MultisampleEnable = cpDesc.m_bMultiSampleEnable;
	//	d3d_desc.AntialiasedLineEnable = cpDesc.m_bMultiSampleEnable;

	//	ID3D11DevicePtr const & d3d_device = checked_cast<D3D11Render*>(&SEContext::Instance().InstFactory().InstRender())->GetD3DDevice();

	//	ID3D11RasterizerState* rasterizer_state;
	//	ErO(d3d_device->CreateRasterizerState(&d3d_desc, &rasterizer_state));
	//	m_pRastState = MakeCOMPtr(rasterizer_state);
	//}
	////激活
	//void D3D11RasterizerStateObj::Active()
	//{
	//	D3D11Render& re = *checked_cast<D3D11Render*>(&SEContext::Instance().InstFactory().InstRender());
	//	re.RSSetState(m_pRastState);
	//}
	////深度模板 参数
	//D3D11DepthStencilStateObj::D3D11DepthStencilStateObj(DepthStencilStateDesc const& cpDesc)
	//	: CDepthStencilStateObj(cpDesc)
	//{
	//	D3D11_DEPTH_STENCIL_DESC d3d_desc;
	//	//深度测试
	//	d3d_desc.DepthEnable = cpDesc.m_bDepthEnable;
	//	//深度模板是否可写
	//	d3d_desc.DepthWriteMask = D3D11Mapping::Mapping(cpDesc.m_bDepthWriteMask);
	//	//深度比较函数
	//	d3d_desc.DepthFunc = D3D11Mapping::Mapping(cpDesc.m_eDepthFunc);
	//	//stencil测试 
	//	d3d_desc.StencilEnable = cpDesc.m_bFrontStencilEnable;
	//	//模板是否可读
	//	d3d_desc.StencilReadMask = static_cast<uint8_t>(cpDesc.m_uFrontStencilReadMask);
	//	d3d_desc.StencilWriteMask = static_cast<uint8_t>(cpDesc.m_uFrontStencilWriteMask);
	//	//如何使用stencil 结果 面对camera
	//	d3d_desc.FrontFace.StencilFailOp = D3D11Mapping::Mapping(cpDesc.m_eFrontStencilFail);
	//	d3d_desc.FrontFace.StencilDepthFailOp = D3D11Mapping::Mapping(cpDesc.m_eBackStencilDepthFail);
	//	d3d_desc.FrontFace.StencilPassOp = D3D11Mapping::Mapping(cpDesc.m_eFrontStencilPass);
	//	d3d_desc.FrontFace.StencilFunc = D3D11Mapping::Mapping(cpDesc.m_eBackStencilFunc);
	//	//背对camera
	//	d3d_desc.BackFace.StencilFailOp = D3D11Mapping::Mapping(cpDesc.m_eBackStencilFail);
	//	d3d_desc.BackFace.StencilDepthFailOp = D3D11Mapping::Mapping(cpDesc.m_eBackStencilDepthFail);
	//	d3d_desc.BackFace.StencilPassOp = D3D11Mapping::Mapping(cpDesc.m_eBackStencilPass);
	//	d3d_desc.BackFace.StencilFunc = D3D11Mapping::Mapping(cpDesc.m_eBackStencilFunc);

	//	ID3D11DevicePtr const& d3d_device = checked_cast<D3D11Render*>(&SEContext::Instance().InstFactory().InstRender())->GetD3DDevice();

	//	ID3D11DepthStencilState* ds_state;
	//	ErO(d3d_device->CreateDepthStencilState(&d3d_desc, &ds_state));
	//	m_pDepthStenState = MakeCOMPtr(ds_state);
	//}
	////深度模板 set到gpu
	//void D3D11DepthStencilStateObj::Active(uint16 uFrontStenRef, uint16 uBackStenRef)
	//{
	//	D3D11Render& re = *checked_cast<D3D11Render*>(&SEContext::Instance().InstFactory().InstRender());
	//	re.OMSetDepthStencilState(m_pDepthStenState, uFrontStenRef);
	//}
	////混合创建
	//D3D11BlendStateObj::D3D11BlendStateObj(BlendStateDesc const& cpDesc)
	//	: CBlendStateObj(cpDesc)
	//{
	//	D3D11_BLEND_DESC d3d_desc;
	//	d3d_desc.AlphaToCoverageEnable = cpDesc.m_bAlphaToCoverageEnable;
	//	d3d_desc.IndependentBlendEnable = cpDesc.m_bIndependentBlendEnable;
	//	for (int i = 0; i < 8; ++ i)
	//	{
	//		d3d_desc.RenderTarget[i].BlendEnable = cpDesc.m_BlendEnables[i];
	//		d3d_desc.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(D3D11Mapping::MappingColorMask(cpDesc.m_ColorWriteMasks[i]));
	//		d3d_desc.RenderTarget[i].SrcBlend = D3D11Mapping::Mapping(cpDesc.m_SrcBlends[i]);
	//		d3d_desc.RenderTarget[i].DestBlend = D3D11Mapping::Mapping(cpDesc.m_DestBlends[i]);
	//		d3d_desc.RenderTarget[i].BlendOp = D3D11Mapping::Mapping(cpDesc.m_BlendOps[i]);
	//		d3d_desc.RenderTarget[i].SrcBlendAlpha = D3D11Mapping::Mapping(cpDesc.m_SrcBlendAlphas[i]);
	//		d3d_desc.RenderTarget[i].DestBlendAlpha = D3D11Mapping::Mapping(cpDesc.m_DestBlendAlphas[i]);
	//		d3d_desc.RenderTarget[i].BlendOpAlpha = D3D11Mapping::Mapping(cpDesc.m_BlendOpAlphas[i]);
	//	}

	//	ID3D11DevicePtr const & d3d_device = checked_cast<D3D11Render*>(&SEContext::Instance().InstFactory().InstRender())->GetD3DDevice();

	//	ID3D11BlendState* blend_state;
	//	ErO(d3d_device->CreateBlendState(&d3d_desc, &blend_state));
	//	m_pBlendState = MakeCOMPtr(blend_state);
	//}
	////blend 激活设置
	//void D3D11BlendStateObj::Active(Color const& cpBlendFactor, uint32 uSampleMask)
	//{
	//	D3D11Render& re = *checked_cast<D3D11Render*>(&SEContext::Instance().InstFactory().InstRender());
	//	//混合状态设置 出入gpu？
	//	re.OMSetBlendState(m_pBlendState, cpBlendFactor, uSampleMask);
	//}
	//D3D11SamplerStateObj::D3D11SamplerStateObj(SamplerStateDesc const& cpDesc)
	//	: CSamplerStateObj(cpDesc)
	//{
	//	D3D11Render const& render_eng = *checked_cast<D3D11Render const *>(&SEContext::Instance().InstFactory().InstRender());
	//	ID3D11DevicePtr const & d3d_device = render_eng.GetD3DDevice();
	//	D3D_FEATURE_LEVEL feature_level = render_eng.DeviceFeatureLevel();

	//	D3D11_SAMPLER_DESC d3d_desc;
	//	d3d_desc.Filter = D3D11Mapping::Mapping(cpDesc.m_eFilter);
	//	d3d_desc.AddressU = D3D11Mapping::Mapping(cpDesc.m_eAddrModeU);
	//	d3d_desc.AddressV = D3D11Mapping::Mapping(cpDesc.m_eAddrModeV);
	//	d3d_desc.AddressW = D3D11Mapping::Mapping(cpDesc.m_eAddrModeW);
	//	d3d_desc.MipLODBias = cpDesc.m_fMipMapLodBias;
	//	d3d_desc.MaxAnisotropy = cpDesc.m_uMaxAnisotropy;
	//	d3d_desc.ComparisonFunc = D3D11Mapping::Mapping(cpDesc.m_eCFunc);
	//	d3d_desc.BorderColor[0] = cpDesc.m_BorderClr.x;
	//	d3d_desc.BorderColor[1] = cpDesc.m_BorderClr.y;
	//	d3d_desc.BorderColor[2] = cpDesc.m_BorderClr.z;
	//	d3d_desc.BorderColor[3] = cpDesc.m_BorderClr.w;
	//	if (feature_level <= D3D_FEATURE_LEVEL_9_3)
	//	{
	//		d3d_desc.MinLOD = floor(cpDesc.m_fMinLod);
	//		d3d_desc.MaxLOD = (std::numeric_limits<float>::max)();
	//	}
	//	else
	//	{
	//		d3d_desc.MinLOD = cpDesc.m_fMinLod;
	//		d3d_desc.MaxLOD = cpDesc.m_fMaxLod;
	//	}

	//	ID3D11SamplerState* sampler_state;
	//	ErO(d3d_device->CreateSamplerState(&d3d_desc, &sampler_state));
	//	m_pSamplerState = MakeCOMPtr(sampler_state);
	//}



}

