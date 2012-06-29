#include <D3D11/D3D11Mapping.h>



namespace SE
{
	//裁剪模式
	D3D11_CULL_MODE D3D11Mapping::Mapping(ECullMode eMode)
	{
		switch (eMode)
		{
		case CM_None:
			return D3D11_CULL_NONE;

		case CM_Front:
			return D3D11_CULL_FRONT;

		case CM_Back:
			return D3D11_CULL_BACK;

		default:
			BOOST_ASSERT(false);
			return D3D11_CULL_NONE;
		}
	}
	//定点链接方法
	D3D11_FILL_MODE D3D11Mapping::Mapping(EPolygonMode eMode)
	{
		switch (eMode)
		{
			//线
		case PM_Point:
			return D3D11_FILL_WIREFRAME;
			//线
		case PM_Line:
			return D3D11_FILL_WIREFRAME;
			//三角形
		case PM_Fill:
			return D3D11_FILL_SOLID;

		default:
			BOOST_ASSERT(false);
			return D3D11_FILL_SOLID;
		}
	}
	//深度贴图 是否可写
	D3D11_DEPTH_WRITE_MASK D3D11Mapping::Mapping(bool bDepthWriteMask)
	{
		if (bDepthWriteMask)
		{
			return D3D11_DEPTH_WRITE_MASK_ALL;
		}
		else
		{
			return D3D11_DEPTH_WRITE_MASK_ZERO;
		}
	}
	//模板设置
	D3D11_STENCIL_OP D3D11Mapping::Mapping(EStencilOperation eOp)
	{
		switch (eOp)
		{
			//保留现有数据
		case SOP_Keep:
			return D3D11_STENCIL_OP_KEEP;
			// set成0
		case SOP_Zero:
			return D3D11_STENCIL_OP_ZERO;
			//值通过 OMSetDepthStencilState.
		case SOP_Replace:
			return D3D11_STENCIL_OP_REPLACE;
			// +1 value
		case SOP_Incr:
			return D3D11_STENCIL_OP_INCR_SAT;
			// -1 value
		case SOP_Decr:
			return D3D11_STENCIL_OP_DECR_SAT;
			//反转
		case SOP_Invert:
			return D3D11_STENCIL_OP_INVERT;
			// +1 后 warp 然后返回
		case SOP_Incr_Wrap:
			return D3D11_STENCIL_OP_INCR;

		case SOP_Decr_Wrap:
			return D3D11_STENCIL_OP_DECR;

		default:
			BOOST_ASSERT(false);
			return D3D11_STENCIL_OP_KEEP;
		};
	}
	//比较函数设置
    D3D11_COMPARISON_FUNC D3D11Mapping::Mapping(ECompareFunction eFunc)
	{
		switch (eFunc)
		{
			//绝不通过比较
		case CF_AlwaysFail:
			return D3D11_COMPARISON_NEVER;
			//总通过
		case CF_AlwaysPass:
			return D3D11_COMPARISON_ALWAYS;
			//小于通过
		case CF_Less:
			return D3D11_COMPARISON_LESS;
			//小于等于通过
		case CF_LessEqual:
			return D3D11_COMPARISON_LESS_EQUAL;

		case CF_Equal:
			return D3D11_COMPARISON_EQUAL;

		case CF_NotEqual:
			return D3D11_COMPARISON_NOT_EQUAL;

		case CF_GreaterEqual:
			return D3D11_COMPARISON_GREATER_EQUAL;

		case CF_Greater:
			return D3D11_COMPARISON_GREATER;

		default:
			BOOST_ASSERT(false);
			return D3D11_COMPARISON_NEVER;
		};
	}
	//颜色标记
	uint32 D3D11Mapping::MappingColorMask(uint32 uMask)
	{
		uint32 ret = 0;
		if (uMask & CMASK_Red)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_RED;
		}
		if (uMask & CMASK_Green)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		}
		if (uMask & CMASK_Blue)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		}
		if (uMask & CMASK_Alpha)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
		}
		return ret;
	}
	//blend alpha 模式
	D3D11_BLEND D3D11Mapping::Mapping(EAlphaBlendFactor eFactor)
	{
		switch (eFactor)
		{
		case ABF_Zero:
			return D3D11_BLEND_ZERO;

		case ABF_One:
			return D3D11_BLEND_ONE;

		case ABF_Src_Alpha:
			return D3D11_BLEND_SRC_ALPHA;

		case ABF_Dst_Alpha:
			//这个是 0-a
			return D3D11_BLEND_DEST_ALPHA;

		case ABF_Inv_Src_Alpha:
			//反转的 1-a
			return D3D11_BLEND_INV_SRC_ALPHA;

		case ABF_Inv_Dst_Alpha:
			return D3D11_BLEND_INV_DEST_ALPHA;

		case ABF_Src_Color:
			return D3D11_BLEND_SRC_COLOR;

		case ABF_Dst_Color:
			return D3D11_BLEND_DEST_COLOR;

		case ABF_Inv_Src_Color:
			return D3D11_BLEND_INV_SRC_COLOR;

		case ABF_Inv_Dst_Color:
			return D3D11_BLEND_INV_DEST_COLOR;

		case ABF_Src_Alpha_Sat:
			return D3D11_BLEND_SRC_ALPHA_SAT;

		default:
			BOOST_ASSERT(false);
			return D3D11_BLEND_ZERO;
		}
	}
	//blend 模式 是+ -  那种
	D3D11_BLEND_OP D3D11Mapping::Mapping(EBlendOperation eBo)
	{
		switch (eBo)
		{
		case BOP_Add:
			return D3D11_BLEND_OP_ADD;

		case BOP_Sub:
			return D3D11_BLEND_OP_SUBTRACT;

		case BOP_Rev_Sub:
			return D3D11_BLEND_OP_REV_SUBTRACT;

		case BOP_Min:
			return D3D11_BLEND_OP_MIN;

		case BOP_Max:
			return D3D11_BLEND_OP_MAX;

		default:
			BOOST_ASSERT(false);
			return D3D11_BLEND_OP_ADD;
		}
	}
	//采样 方法 dian 线性
	D3D11_FILTER D3D11Mapping::Mapping(ETexFilterOp eFilter)
	{
		switch (eFilter)
		{
		case TFO_Min_Mag_Mip_Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;

		case TFO_Min_Mag_Point_Mip_Linear:
			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		case TFO_Min_Point_Mag_Linear_Mip_Point:
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

		case TFO_Min_Point_Mag_Mip_Linear:
			return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

		case TFO_Min_Mag_Linear_Mip_Point:
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		case TFO_Min_Mag_Mip_Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		case TFO_Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;

		case TFO_Cmp_Min_Mag_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

		case TFO_Cmp_Min_Mag_Point_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;

		case TFO_Cmp_Min_Point_Mag_Linear_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;

		case TFO_Cmp_Min_Point_Mag_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;

		case TFO_Cmp_Min_Mag_Linear_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

		case TFO_Cmp_Min_Mag_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

		case TFO_Cmp_Anisotropic:
			return D3D11_FILTER_COMPARISON_ANISOTROPIC;

		default:
			BOOST_ASSERT(false);
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}
	}
	//纹理 坐标 设定
	D3D11_TEXTURE_ADDRESS_MODE D3D11Mapping::Mapping(ETexAddressingMode eMode)
	{
		switch (eMode)
		{
		case TAM_Clamp:
			//0-3的纹理坐标 就是 纹理重复三次
			return D3D11_TEXTURE_ADDRESS_CLAMP;
			//反转纹理 在   0-1 正1-2 反 2-3 正
		case TAM_Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			// 纹理超过 0 或1 设置成 0 或 1的颜色
		case TAM_Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
			//设置超出 0- 1 范围的指定颜色
		case TAM_Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;

		default:
			BOOST_ASSERT(false);
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		}
	}
	//像素格式映射成d3d的
	DXGI_FORMAT D3D11Mapping::MappingFormat(PixelFormat PFmt)
	{
		switch (PFmt)
		{
		case EF_A8:
			return DXGI_FORMAT_A8_UNORM;

		case EF_R8:
			return DXGI_FORMAT_R8_UNORM;

		case EF_SIGNED_R8:
			return DXGI_FORMAT_R8_SNORM;

		case EF_GR8:
			return DXGI_FORMAT_R8G8_UNORM;

		case EF_SIGNED_GR8:
			return DXGI_FORMAT_R8G8_SNORM;

		case EF_ARGB8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;

		case EF_ABGR8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case EF_SIGNED_ABGR8:
			return DXGI_FORMAT_R8G8B8A8_SNORM;

		case EF_A2BGR10:
			return DXGI_FORMAT_R10G10B10A2_UNORM;

		case EF_R8UI:
			return DXGI_FORMAT_R8_UINT;

		case EF_R8I:
			return DXGI_FORMAT_R8_SINT;

		case EF_GR8UI:
			return DXGI_FORMAT_R8G8_UINT;

		case EF_GR8I:
			return DXGI_FORMAT_R8G8_SINT;

		case EF_ABGR8UI:
			return DXGI_FORMAT_R8G8B8A8_UINT;

		case EF_ABGR8I:
			return DXGI_FORMAT_R8G8B8A8_SINT;

		case EF_A2BGR10UI:
			return DXGI_FORMAT_R10G10B10A2_UINT;

		case EF_R16:
			return DXGI_FORMAT_R16_UNORM;

		case EF_SIGNED_R16:
			return DXGI_FORMAT_R16_SNORM;

		case EF_GR16:
			return DXGI_FORMAT_R16G16_UNORM;

		case EF_SIGNED_GR16:
			return DXGI_FORMAT_R16G16_SNORM;

		case EF_ABGR16:
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case EF_SIGNED_ABGR16:
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case EF_R16UI:
			return DXGI_FORMAT_R16_UINT;

		case EF_R16I:
			return DXGI_FORMAT_R16_SINT;

		case EF_GR16UI:
			return DXGI_FORMAT_R16G16_UINT;

		case EF_GR16I:
			return DXGI_FORMAT_R16G16_SINT;

		case EF_ABGR16UI:
			return DXGI_FORMAT_R16G16B16A16_UINT;

		case EF_ABGR16I:
			return DXGI_FORMAT_R16G16B16A16_SINT;

		case EF_R32UI:
			return DXGI_FORMAT_R32_UINT;

		case EF_R32I:
			return DXGI_FORMAT_R32_SINT;

		case EF_GR32UI:
			return DXGI_FORMAT_R32G32_UINT;

		case EF_GR32I:
			return DXGI_FORMAT_R32G32_SINT;

		case EF_BGR32UI:
			return DXGI_FORMAT_R32G32B32_UINT;

		case EF_BGR32I:
			return DXGI_FORMAT_R32G32B32_SINT;

		case EF_ABGR32UI:
			return DXGI_FORMAT_R32G32B32A32_UINT;

		case EF_ABGR32I:
			return DXGI_FORMAT_R32G32B32A32_SINT;

		case EF_R16F:
			return DXGI_FORMAT_R16_FLOAT;

		case EF_GR16F:
			return DXGI_FORMAT_R16G16_FLOAT;

		case EF_B10G11R11F:
			return DXGI_FORMAT_R11G11B10_FLOAT;

		case EF_ABGR16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case EF_R32F:
			return DXGI_FORMAT_R32_FLOAT;

		case EF_GR32F:
			return DXGI_FORMAT_R32G32_FLOAT;

		case EF_BGR32F:
			return DXGI_FORMAT_R32G32B32_FLOAT;

		case EF_ABGR32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case EF_BC1:
			return DXGI_FORMAT_BC1_UNORM;

		case EF_BC2:
			return DXGI_FORMAT_BC2_UNORM;

		case EF_BC3:
			return DXGI_FORMAT_BC3_UNORM;

		case EF_BC4:
			return DXGI_FORMAT_BC4_UNORM;

		case EF_SIGNED_BC4:
			return DXGI_FORMAT_BC4_SNORM;

		case EF_BC5:
			return DXGI_FORMAT_BC5_UNORM;

		case EF_SIGNED_BC5:
			return DXGI_FORMAT_BC5_SNORM;

		case EF_BC6:
			return DXGI_FORMAT_BC6H_UF16;

		case EF_SIGNED_BC6:
			return DXGI_FORMAT_BC6H_SF16;

		case EF_BC7:
			return DXGI_FORMAT_BC7_UNORM;

		case EF_D16:
			return DXGI_FORMAT_D16_UNORM;

		case EF_D24S8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;

		case EF_D32F:
			return DXGI_FORMAT_D32_FLOAT;
			
		case EF_ARGB8_SRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

		case EF_ABGR8_SRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		case EF_BC1_SRGB:
			return DXGI_FORMAT_BC1_UNORM_SRGB;

		case EF_BC2_SRGB:
			return DXGI_FORMAT_BC2_UNORM_SRGB;

		case EF_BC3_SRGB:
			return DXGI_FORMAT_BC3_UNORM_SRGB;

		case EF_BC7_SRGB:
			return DXGI_FORMAT_BC7_UNORM_SRGB;

		default:
			THR(boost::system::posix_error::not_supported);
		}
	}
    PixelFormat D3D11Mapping::MappingFormat(DXGI_FORMAT D3DFmt)
	{
		
#ifdef SE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4063)
#endif
		switch (D3DFmt)
		{
		case DXGI_FORMAT_A8_UNORM:
			return EF_A8;

		case DXGI_FORMAT_R8_UNORM:
			return EF_R8;

		case DXGI_FORMAT_R8_SNORM:
			return EF_SIGNED_R8;

		case DXGI_FORMAT_R8G8_UNORM:
			return EF_GR8;

		case DXGI_FORMAT_R8G8_SNORM:
			return EF_SIGNED_GR8;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return EF_ARGB8;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return EF_ABGR8;

		case DXGI_FORMAT_R8G8B8A8_SNORM:
			return EF_SIGNED_ABGR8;

		case DXGI_FORMAT_R10G10B10A2_UNORM:
			return EF_A2BGR10;

		case DXGI_FORMAT_R8_UINT:
			return EF_R8UI;

		case DXGI_FORMAT_R8_SINT:
			return EF_R8I;

		case DXGI_FORMAT_R8G8_UINT:
			return EF_GR8UI;

		case DXGI_FORMAT_R8G8_SINT:
			return EF_GR8I;

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return EF_ABGR8UI;

		case DXGI_FORMAT_R8G8B8A8_SINT:
			return EF_ABGR8I;

		case DXGI_FORMAT_R10G10B10A2_UINT:
			return EF_A2BGR10UI;

		case DXGI_FORMAT_R16_UNORM:
			return EF_R16;

		case DXGI_FORMAT_R16_SNORM:
			return EF_SIGNED_R16;

		case DXGI_FORMAT_R16G16_UNORM:
			return EF_GR16;

		case DXGI_FORMAT_R16G16_SNORM:
			return EF_SIGNED_GR16;

		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return EF_ABGR16;

		case DXGI_FORMAT_R16G16B16A16_SNORM:
			return EF_SIGNED_ABGR16;

		case DXGI_FORMAT_R16_UINT:
			return EF_R16UI;

		case DXGI_FORMAT_R16_SINT:
			return EF_R16I;

		case DXGI_FORMAT_R16G16_UINT:
			return EF_GR16UI;

		case DXGI_FORMAT_R16G16_SINT:
			return EF_GR16I;

		case DXGI_FORMAT_R16G16B16A16_UINT:
			return EF_ABGR16UI;

		case DXGI_FORMAT_R16G16B16A16_SINT:
			return EF_ABGR16I;

		case DXGI_FORMAT_R32_UINT:
			return EF_R32UI;

		case DXGI_FORMAT_R32_SINT:
			return EF_R32I;

		case DXGI_FORMAT_R32G32_UINT:
			return EF_GR32UI;

		case DXGI_FORMAT_R32G32_SINT:
			return EF_GR32I;

		case DXGI_FORMAT_R32G32B32_UINT:
			return EF_BGR32UI;

		case DXGI_FORMAT_R32G32B32_SINT:
			return EF_BGR32I;

		case DXGI_FORMAT_R32G32B32A32_UINT:
			return EF_ABGR32UI;

		case DXGI_FORMAT_R32G32B32A32_SINT:
			return EF_ABGR32I;

		case DXGI_FORMAT_R16_FLOAT:
			return EF_R16F;

		case DXGI_FORMAT_R16G16_FLOAT:
			return EF_GR16F;

		case DXGI_FORMAT_R11G11B10_FLOAT:
			return EF_B10G11R11F;

		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return EF_ABGR16F;

		case DXGI_FORMAT_R32_FLOAT:
			return EF_R32F;

		case DXGI_FORMAT_R32G32_FLOAT:
			return EF_GR32F;

		case DXGI_FORMAT_R32G32B32_FLOAT:
			return EF_BGR32F;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return EF_ABGR32F;

		case DXGI_FORMAT_BC1_UNORM:
			return EF_BC1;

		case DXGI_FORMAT_BC2_UNORM:
			return EF_BC2;

		case DXGI_FORMAT_BC3_UNORM:
			return EF_BC3;

		case DXGI_FORMAT_BC4_UNORM:
			return EF_BC4;

		case DXGI_FORMAT_BC4_SNORM:
			return EF_SIGNED_BC4;

		case DXGI_FORMAT_BC5_UNORM:
			return EF_BC5;

		case DXGI_FORMAT_BC5_SNORM:
			return EF_SIGNED_BC5;

		case DXGI_FORMAT_BC6H_UF16:
			return EF_BC6;

		case DXGI_FORMAT_BC6H_SF16:
			return EF_SIGNED_BC6;

		case DXGI_FORMAT_BC7_UNORM:
			return EF_BC7;

		case DXGI_FORMAT_D16_UNORM:
			return EF_D16;

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return EF_D24S8;

		case DXGI_FORMAT_D32_FLOAT:
			return EF_D32F;

		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return EF_ARGB8_SRGB;

		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return EF_ABGR8_SRGB;

		case DXGI_FORMAT_BC1_UNORM_SRGB:
			return EF_BC1_SRGB;

		case DXGI_FORMAT_BC2_UNORM_SRGB:
			return EF_BC2_SRGB;

		case DXGI_FORMAT_BC3_UNORM_SRGB:
			return EF_BC3_SRGB;

		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return EF_BC7_SRGB;

		default:
			THR(boost::system::posix_error::not_supported);
		}
#ifdef SE_COMPILER_MSVC
#pragma warning(pop)
#endif
	}


}