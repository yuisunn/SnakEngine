///egine 映射 opengles 插件的一些东西
#ifndef _D3D11Mapping_H_
#define _D3D11Mapping_H_

#include <d3d11.h>
#include <SE3DEngine/RenderSystem/CRenderStateObj.h>
#include <SE3DEngine/Comm/PixelFormat.h>

namespace SE
{
    class _DLL_API D3D11Mapping
    {
	public:
		static D3D11_BLEND Mapping(EAlphaBlendFactor eFactor);
		static D3D11_CULL_MODE Mapping(ECullMode eMode);
		static D3D11_FILL_MODE Mapping(EPolygonMode eMode);
		static D3D11_DEPTH_WRITE_MASK Mapping(bool bDepthWriteMask);
		static D3D11_COMPARISON_FUNC Mapping(ECompareFunction eFunc);
		static uint32 MappingColorMask(uint32 uMask);
		static D3D11_FILTER Mapping(ETexFilterOp eFilter);
		static D3D11_BLEND_OP Mapping(EBlendOperation eBo);
		static D3D11_STENCIL_OP Mapping(EStencilOperation eOp);
		static D3D11_TEXTURE_ADDRESS_MODE D3D11Mapping::Mapping(ETexAddressingMode eMode);
		static DXGI_FORMAT MappingFormat(PixelFormat PFmt);
		static PixelFormat MappingFormat(DXGI_FORMAT D3DFmt);

		/*

		static D3D11_MAP Mapping(TextureMapAccess tma, Texture::TextureType type, uint32_t access_hint, uint32_t numMipMaps);
		

		static D3D11_PRIMITIVE_TOPOLOGY Mapping(RenderLayout::topology_type tt);
		static void Mapping(std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, size_t stream, vertex_elements_type const & vet, RenderLayout::stream_type type, uint32_t freq);

		static D3D11_SO_DECLARATION_ENTRY Mapping(shader_desc::stream_output_decl const & decl, uint8_t slot);
		*/

    };
}

#endif
