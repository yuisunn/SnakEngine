#ifndef _RenderStateObj_H_
#define _RenderStateObj_H_

#include <SE3DEngine/Comm/Def.h>
#include <boost/array.hpp>

namespace SE
{
	inline uint32 FloatToUint32(float v)
	{
		union FNU
		{
			float f;
			uint32 u;
		} fnu;
		fnu.f = v;
		return fnu.u;
	}
	inline float Uint32ToFloat(uint32 v)
	{
		union FNU
		{
			uint32 u;
			float f;
		} unf;
		unf.u = v;
		return unf.f;
	}

	enum EPolygonMode
	{
		PM_Point,
		PM_Line,
		PM_Fill
	};

	enum EShadeMode
	{
		SM_Flat,
		SM_Gouraud
	};

	enum ECullMode
	{
		CM_None,
		CM_Front,
		CM_Back
	};

	enum EBlendOperation
	{
		BOP_Add		= 1,
		BOP_Sub		= 2,
		BOP_Rev_Sub	= 3,
		BOP_Min		= 4,
		BOP_Max		= 5,
	};

	enum EAlphaBlendFactor
	{
		ABF_Zero,
		ABF_One,
		ABF_Src_Alpha,
		ABF_Dst_Alpha,
		ABF_Inv_Src_Alpha,
		ABF_Inv_Dst_Alpha,
		ABF_Src_Color,
		ABF_Dst_Color,
		ABF_Inv_Src_Color,
		ABF_Inv_Dst_Color,
		ABF_Src_Alpha_Sat
	};

	enum ECompareFunction
	{
		CF_AlwaysFail,
		CF_AlwaysPass,
		CF_Less,
		CF_LessEqual,
		CF_Equal,
		CF_NotEqual,
		CF_GreaterEqual,
		CF_Greater
	};

	// Enum describing the various actions which can be taken onthe stencil buffer
	enum EStencilOperation
	{
		// Leave the stencil buffer unchanged
		SOP_Keep,
		// Set the stencil value to zero
		SOP_Zero,
		// Set the stencil value to the reference value
		SOP_Replace,
		// Increase the stencil value by 1, clamping at the maximum value
		SOP_Incr,
		// Decrease the stencil value by 1, clamping at 0
		SOP_Decr,
		// Invert the bits of the stencil buffer
		SOP_Invert,
		// Increase the stencil value by 1, wrap the result if necessary
		SOP_Incr_Wrap,
		// Decrease the stencil value by 1, wrap the result if necessary
		SOP_Decr_Wrap
	};

	enum EColorMask
	{
		CMASK_Red   = 1UL << 3,
		CMASK_Green = 1UL << 2,
		CMASK_Blue  = 1UL << 1,
		CMASK_Alpha = 1UL << 0,
		CMASK_All   = CMASK_Red | CMASK_Green | CMASK_Blue | CMASK_Alpha
	};

	// Sampler addressing modes - default is TAM_Wrap.
	enum ETexAddressingMode
	{
		// Texture wraps at values over 1.0
		TAM_Wrap,
		// Texture mirrors (flips) at joins over 1.0
		TAM_Mirror,
		// Texture clamps at 1.0
		TAM_Clamp,
		// Texture coordinates outside the range [0.0, 1.0] are set to the border color.
		TAM_Border
	};

	enum ETexFilterOp
	{
		// Dont' use these enum directly
		TFOE_Mip_Point = 0x0,
		TFOE_Mip_Linear = 0x1,
		TFOE_Mag_Point = 0x0,
		TFOE_Mag_Linear = 0x2,
		TFOE_Min_Point = 0x0,
		TFOE_Min_Linear = 0x4,
		TFOE_Anisotropic = 0x0F,
		TFOE_Comparison = 0x10,

		// Use these
		TFO_Min_Mag_Mip_Point				= TFOE_Min_Point  | TFOE_Mag_Point  | TFOE_Mip_Point,
		TFO_Min_Mag_Point_Mip_Linear		= TFOE_Min_Point  | TFOE_Mag_Point  | TFOE_Mip_Linear,
		TFO_Min_Point_Mag_Linear_Mip_Point	= TFOE_Min_Point  | TFOE_Mag_Linear | TFOE_Mip_Point,
		TFO_Min_Point_Mag_Mip_Linear		= TFOE_Min_Point  | TFOE_Mag_Linear | TFOE_Mip_Linear,
		TFO_Min_Linear_Mag_Mip_Point		= TFOE_Min_Linear | TFOE_Mag_Point  | TFOE_Mip_Point,
		TFO_Min_Linear_Mag_Point_Mip_Linear	= TFOE_Min_Linear | TFOE_Mag_Point  | TFOE_Mip_Linear,
		TFO_Min_Mag_Linear_Mip_Point		= TFOE_Min_Linear | TFOE_Mag_Linear | TFOE_Mip_Point,
		TFO_Min_Mag_Mip_Linear				= TFOE_Min_Linear | TFOE_Mag_Linear | TFOE_Mip_Linear,
		TFO_Anisotropic						= TFOE_Anisotropic,

		TFO_Cmp_Min_Mag_Mip_Point				= TFOE_Comparison | TFO_Min_Mag_Mip_Point,
		TFO_Cmp_Min_Mag_Point_Mip_Linear		= TFOE_Comparison | TFO_Min_Mag_Point_Mip_Linear,
		TFO_Cmp_Min_Point_Mag_Linear_Mip_Point	= TFOE_Comparison | TFO_Min_Point_Mag_Linear_Mip_Point,
		TFO_Cmp_Min_Point_Mag_Mip_Linear		= TFOE_Comparison | TFO_Min_Point_Mag_Mip_Linear,
		TFO_Cmp_Min_Linear_Mag_Mip_Point		= TFOE_Comparison | TFO_Min_Linear_Mag_Mip_Point,
		TFO_Cmp_Min_Linear_Mag_Point_Mip_Linear	= TFOE_Comparison | TFO_Min_Linear_Mag_Point_Mip_Linear,
		TFO_Cmp_Min_Mag_Linear_Mip_Point		= TFOE_Comparison | TFO_Min_Mag_Linear_Mip_Point,
		TFO_Cmp_Min_Mag_Mip_Linear				= TFOE_Comparison | TFO_Min_Mag_Mip_Linear,
		TFO_Cmp_Anisotropic						= TFOE_Comparison | TFO_Anisotropic
	};

#ifdef SE_WINDOWS
#pragma pack(push, 1)
#endif

   	struct DLL_API  RasterizerStateDesc
	{
		EPolygonMode		m_ePolygonMode;
		EShadeMode			m_eShadeMode;
		ECullMode			m_eCullMode;
		bool				m_bFrontFaceCCW;
		float				m_fPolygonOffsetFactor;
		float				m_fPolygonOffsetUnits;
		bool				m_bDepthClipEnable;
		bool				m_bScissorEnable;
		bool				m_bMultiSampleEnable;

		RasterizerStateDesc();

		friend bool operator<(RasterizerStateDesc const & cpLhs, RasterizerStateDesc const & cpRhs);
	};

	struct DLL_API  DepthStencilStateDesc
	{
		bool				m_bDepthEnable;
		bool				m_bDepthWriteMask;
		ECompareFunction	m_eDepthFunc;

		bool				m_bFrontStencilEnable;
		ECompareFunction	m_eFrontStencilFunc;
		uint16			    m_uFrontStencilReadMask;
		uint16			    m_uFrontStencilWriteMask;
		EStencilOperation	m_eFrontStencilFail;
		EStencilOperation	m_eFrontStencilDepthFail;
		EStencilOperation	m_eFrontStencilPass;

		bool				m_bBackStencilEnable;
		ECompareFunction	m_eBackStencilFunc;
		uint16			    m_uBackStencilReadMask;
		uint16			    m_uBackStencilWriteMask;
		EStencilOperation	m_eBackStencilFail;
		EStencilOperation	m_eBackStencilDepthFail;
		EStencilOperation	m_eBackStencilPass;

		DepthStencilStateDesc();

		friend bool operator<(DepthStencilStateDesc const & cpLhs, DepthStencilStateDesc const & cpRhs);
	};

	struct DLL_API BlendStateDesc
	{
		bool				m_bAlphaToCoverageEnable;
		bool				m_bIndependentBlendEnable;

		boost::array<bool, 8>				m_BlendEnables;
		boost::array<EBlendOperation, 8>		m_BlendOps;
		boost::array<EAlphaBlendFactor, 8>	m_SrcBlends;
		boost::array<EAlphaBlendFactor, 8>	m_DestBlends;
		boost::array<EBlendOperation, 8>		m_BlendOpAlphas;
		boost::array<EAlphaBlendFactor, 8>	m_SrcBlendAlphas;
		boost::array<EAlphaBlendFactor, 8>	m_DestBlendAlphas;
		boost::array<uint8, 8>			m_ColorWriteMasks;

		BlendStateDesc();

		friend bool operator<(BlendStateDesc const & cpLhs, BlendStateDesc const & cpRhs);
	};

	struct DLL_API  SamplerStateDesc
	{
		Color m_BorderClr;

		ETexAddressingMode m_eAddrModeU;
		ETexAddressingMode m_eAddrModeV;
		ETexAddressingMode m_eAddrModeW;

		ETexFilterOp m_eFilter;

		uint8 m_uMaxAnisotropy;
		float m_fMinLod;
		float m_fMaxLod;
		float m_fMipMapLodBias;

		ECompareFunction m_eCFunc;

		SamplerStateDesc();
		SamplerStateDesc(const SamplerStateDesc&);

		friend bool operator<(SamplerStateDesc const & cpLhs, SamplerStateDesc const & cpRhs);
	};
#ifdef SE_WINDOWS
#pragma pack(pop)
#endif

	class DLL_API CRasterizerStateObj
	{
	public:
		explicit CRasterizerStateObj(RasterizerStateDesc const & cpDesc)
			: m_Desc(cpDesc)
		{
		}

		virtual ~CRasterizerStateObj()
		{
		}

		RasterizerStateDesc const & GetDesc() const
		{
			return m_Desc;
		}

		static CRasterizerStateObjPtr NullObject();

		virtual void Active() = 0;

	protected:
		RasterizerStateDesc m_Desc;
	};

	class DLL_API CDepthStencilStateObj
	{
	public:
		explicit CDepthStencilStateObj(DepthStencilStateDesc const& cpDesc)
			: m_Desc(cpDesc)
		{
		}

		virtual ~CDepthStencilStateObj()
		{
		}

		DepthStencilStateDesc const & GetDesc() const
		{
			return m_Desc;
		}

		static CDepthStencilStateObjPtr NullObject();

		virtual void Active(uint16 uFrontStencilRef, uint16 uBackStencilRef) = 0;

	protected:
		DepthStencilStateDesc m_Desc;
	};

	class DLL_API CBlendStateObj
	{
	public:
		explicit CBlendStateObj(BlendStateDesc const & cpDesc)
			: m_Desc(cpDesc)
		{
		}

		virtual ~CBlendStateObj()
		{
		}

		BlendStateDesc const & GetDesc() const
		{
			return m_Desc;
		}

		static CBlendStateObjPtr NullObject();

		virtual void Active(Color const & cpBlendFactor, uint32 uSampleMask) = 0;

	protected:
		BlendStateDesc m_Desc;
	};

	class DLL_API CSamplerStateObj
	{
	public:
		explicit CSamplerStateObj(SamplerStateDesc const & cpDesc)
		         //: m_Desc(cpDesc)
		{
		}

		virtual ~CSamplerStateObj()
		{
		}

		SamplerStateDesc const & GetDesc() const
		{
			return m_Desc;
		}

		static CSamplerStateObjPtr NullObject();

	protected:
		SamplerStateDesc m_Desc;
	};
}
#endif

