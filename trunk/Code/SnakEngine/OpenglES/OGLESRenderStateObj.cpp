#include <OpenglES/OGLESRenderStateObj.h>

#include <GLES2/gl2ext.h>

#include <SE3DEngine/Comm/Util.h>
#include <SE3DEngine/Comm/SEContext.h>

#include <OpenglES/OGLESMapping.h>
#include <OpenglES/OGLESTexture.h>

 
//#include <gl\GL.h>


namespace SE
{
	OGLESRasterizerStateObj::OGLESRasterizerStateObj(RasterizerStateDesc const & cpDesc)
		: CRasterizerStateObj(cpDesc),
		//“this”: 用于基成员初始值设定项列表
			m_eGlFrontFace(this->m_Desc.m_bFrontFaceCCW ? GL_CCW : GL_CW)
	{
	}

	void OGLESRasterizerStateObj::Active()
	{
		OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());

		RasterizerStateDesc const & cur_desc = re.CurRasStateObj()->GetDesc();

		if (cur_desc.m_eCullMode != m_Desc.m_eCullMode)
		{
			switch (m_Desc.m_eCullMode)
			{
			case CM_None:
				glDisable(GL_CULL_FACE);
				break;

			case CM_Front:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;

			case CM_Back:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;
			}
		}

		if (cur_desc.m_bFrontFaceCCW != m_Desc.m_bFrontFaceCCW)
		{
			glFrontFace(m_eGlFrontFace);
		}

		if ((cur_desc.m_fPolygonOffsetFactor != m_Desc.m_fPolygonOffsetFactor)
			|| (cur_desc.m_fPolygonOffsetUnits != m_Desc.m_fPolygonOffsetUnits))
		{
			// Bias is in {0, 16}, scale the unit addition appropriately
			glPolygonOffset(m_Desc.m_fPolygonOffsetFactor, m_Desc.m_fPolygonOffsetUnits);
		}

		if (cur_desc.m_bScissorEnable != m_Desc.m_bScissorEnable)
		{
			if (m_Desc.m_bScissorEnable)
			{
				glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				glDisable(GL_SCISSOR_TEST);
			}
		}
	}

	void OGLESRasterizerStateObj::ForceDefaultState()
	{
		RasterizerStateDesc desc;

		switch (desc.m_eCullMode)
		{
		case CM_None:
			glDisable(GL_CULL_FACE);
			break;

		case CM_Front:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;

		case CM_Back:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		}

		glFrontFace(desc.m_bFrontFaceCCW ? GL_CCW : GL_CW);

		glPolygonOffset(desc.m_fPolygonOffsetFactor, desc.m_fPolygonOffsetUnits);

		if (desc.m_bScissorEnable)
		{
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}


	OGLESDepthStencilStateObj::OGLESDepthStencilStateObj(DepthStencilStateDesc const & cpDesc)
		: CDepthStencilStateObj(cpDesc),
		m_bGlDepthWriteMask(m_Desc.m_bDepthWriteMask ? GL_TRUE : GL_FALSE),
		m_eGlDepthFunc(OGLESMapping::Mapping(m_Desc.m_eDepthFunc)),
		m_eGlFrontStencilFunc(OGLESMapping::Mapping(m_Desc.m_eFrontStencilFunc)),
		m_eGlFrontStencilFail(OGLESMapping::Mapping(m_Desc.m_eBackStencilDepthFail)),
		m_eGlFrontStencilDepthFail(OGLESMapping::Mapping(m_Desc.m_eBackStencilDepthFail)),
		m_eGlFrontStencilPass(OGLESMapping::Mapping(m_Desc.m_eFrontStencilPass)),
		m_eGlBackStencilFunc(OGLESMapping::Mapping(m_Desc.m_eBackStencilFunc)),
		m_eGlBackStencilFail(OGLESMapping::Mapping(m_Desc.m_eBackStencilFail)),
		m_eGlBackStencilDepthFail(OGLESMapping::Mapping(m_Desc.m_eBackStencilDepthFail)),
		m_eGlBackStencilPass(OGLESMapping::Mapping(m_Desc.m_eBackStencilPass))
	{
	}

	void OGLESDepthStencilStateObj::Active(uint16 uFrontStencilRef, uint16 uBackStencilRef)
	{
		OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());

		DepthStencilStateDesc const & cur_desc = re.CurDepthStenStateObj()->GetDesc();
		uint16 const cur_front_stencil_ref = re.CurFrontStencilRef();
		uint16 const cur_back_stencil_ref = re.CurBackStencilRef();

		if (cur_desc.m_bDepthEnable != m_Desc.m_bDepthEnable)
		{
			if (m_Desc.m_bDepthEnable)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}
		if (cur_desc.m_bDepthWriteMask != m_Desc.m_bDepthWriteMask)
		{
			glDepthMask(m_bGlDepthWriteMask);
		}
		if (cur_desc.m_eDepthFunc != m_Desc.m_eDepthFunc)
		{
			glDepthFunc(m_eGlDepthFunc);
		}

		if ((cur_desc.m_eFrontStencilFunc != m_Desc.m_eFrontStencilFunc)
			|| (cur_front_stencil_ref != uFrontStencilRef)
			|| (cur_desc.m_uFrontStencilReadMask != m_Desc.m_uFrontStencilReadMask))
		{
			glStencilFuncSeparate(GL_FRONT, m_eGlFrontStencilFunc,
					uFrontStencilRef, m_Desc.m_uFrontStencilReadMask);
		}
		if ((cur_desc.m_eFrontStencilFail != m_Desc.m_eFrontStencilFail)
			|| (cur_desc.m_eFrontStencilDepthFail != m_Desc.m_eFrontStencilDepthFail)
			|| (cur_desc.m_eFrontStencilPass != m_Desc.m_eFrontStencilPass))
		{
			glStencilOpSeparate(GL_FRONT, m_eGlFrontStencilFail,
					m_eGlFrontStencilDepthFail, m_eGlFrontStencilPass);
		}
		if (cur_desc.m_uFrontStencilWriteMask != m_Desc.m_uFrontStencilWriteMask)
		{
			glStencilMaskSeparate(GL_FRONT, m_Desc.m_uFrontStencilWriteMask);
		}

		if ((cur_desc.m_eBackStencilFunc != m_Desc.m_eBackStencilFunc)
			|| (cur_back_stencil_ref != uBackStencilRef)
			|| (cur_desc.m_uBackStencilReadMask != m_Desc.m_uBackStencilReadMask))
		{
			glStencilFuncSeparate(GL_BACK, m_eGlBackStencilFunc,
					uBackStencilRef, m_Desc.m_uBackStencilReadMask);
		}
		if ((cur_desc.m_eBackStencilFail != m_Desc.m_eBackStencilFail)
			|| (cur_desc.m_eBackStencilDepthFail != m_Desc.m_eBackStencilDepthFail)
			|| (cur_desc.m_eBackStencilPass != m_Desc.m_eBackStencilPass))
		{
			glStencilOpSeparate(GL_BACK, m_eGlBackStencilFail,
					m_eGlBackStencilDepthFail, m_eGlBackStencilPass);
		}
		if (cur_desc.m_uBackStencilWriteMask != m_Desc.m_uBackStencilWriteMask)
		{
			glStencilMaskSeparate(GL_BACK, m_Desc.m_uBackStencilWriteMask);
		}

		if ((cur_desc.m_bFrontStencilEnable != m_Desc.m_bFrontStencilEnable)
			|| (cur_desc.m_bBackStencilEnable != m_Desc.m_bBackStencilEnable))
		{
			if (m_Desc.m_bFrontStencilEnable || m_Desc.m_bFrontStencilEnable)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}

	void OGLESDepthStencilStateObj::ForceDefaultState()
	{
		DepthStencilStateDesc desc;

		if (desc.m_bDepthEnable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		//关闭还是开启 depth buffer
		glDepthMask(desc.m_bDepthWriteMask ? GL_TRUE : GL_FALSE);
		//定义深度比较函数 
		glDepthFunc(OGLESMapping::Mapping(desc.m_eDepthFunc));
		
		glStencilFuncSeparate(GL_FRONT, OGLESMapping::Mapping(desc.m_eFrontStencilFunc),
				0, desc.m_uFrontStencilReadMask);
		//定义stencil pass faild 后的操作
		glStencilOpSeparate(GL_FRONT, OGLESMapping::Mapping(desc.m_eFrontStencilFail),
				OGLESMapping::Mapping(desc.m_eFrontStencilDepthFail), OGLESMapping::Mapping(desc.m_eFrontStencilPass));
	    //定义对那个面stencil 做操作
		glStencilMaskSeparate(GL_FRONT, desc.m_uFrontStencilWriteMask);
		//设置映射值
		glStencilFuncSeparate(GL_BACK, OGLESMapping::Mapping(desc.m_eBackStencilFunc),
				0, desc.m_uBackStencilReadMask);
		glStencilOpSeparate(GL_BACK, OGLESMapping::Mapping(desc.m_eBackStencilFail),
				OGLESMapping::Mapping(desc.m_eBackStencilDepthFail), OGLESMapping::Mapping(desc.m_eBackStencilPass));
		glStencilMaskSeparate(GL_BACK, desc.m_uBackStencilWriteMask);

		if (desc.m_bFrontStencilEnable || desc.m_bBackStencilEnable)
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}


	OGLESBlendStateObj::OGLESBlendStateObj(BlendStateDesc const & cpDesc)
		: CBlendStateObj(cpDesc),
			m_eGlBlendOp(OGLESMapping::Mapping(m_Desc.m_BlendOps[0])),
			m_eGlBlendOpAlpha(OGLESMapping::Mapping(m_Desc.m_BlendOpAlphas[0])),
			m_eGlSrcBlend(OGLESMapping::Mapping(m_Desc.m_SrcBlends[0])),
			m_eGlDestBlend(OGLESMapping::Mapping(m_Desc.m_DestBlends[0])),
			m_eGlSrcBlendAlpha(OGLESMapping::Mapping(m_Desc.m_SrcBlendAlphas[0])),
			m_eGlDestBlendAlpha(OGLESMapping::Mapping(m_Desc.m_DestBlendAlphas[0]))
	{
	}

	void OGLESBlendStateObj::Active(Color const & cpBlendFactor, uint32 uSampleMask)
	{
		OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());

		BlendStateDesc const& cur_desc = re.CurBlendStateObj()->GetDesc();
		Color const& cur_blend_factor = re.CurBlendFactor();

		if (cur_desc.m_bAlphaToCoverageEnable != m_Desc.m_bAlphaToCoverageEnable)
		{
			if (m_Desc.m_bAlphaToCoverageEnable)
			{
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
			else
			{
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
		}
		if (cur_desc.m_BlendEnables[0] != m_Desc.m_BlendEnables[0])
		{
			if (m_Desc.m_BlendEnables[0])
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}

		if (cur_desc.m_BlendOps[0] != m_Desc.m_BlendOps[0])
		{
			glBlendEquationSeparate(m_eGlBlendOp, m_eGlBlendOpAlpha);
		}
		if ((cur_desc.m_SrcBlends[0] != m_Desc.m_SrcBlends[0])
			|| (cur_desc.m_DestBlends[0] != m_Desc.m_DestBlends[0])
			|| (cur_desc.m_SrcBlendAlphas[0] != m_Desc.m_SrcBlendAlphas[0])
			|| (cur_desc.m_DestBlendAlphas[0] != m_Desc.m_DestBlendAlphas[0]))
		{
			//blend 混合 rgb 和 alpha
			glBlendFuncSeparate(m_eGlSrcBlend, m_eGlDestBlend,
					m_eGlSrcBlendAlpha, m_eGlDestBlendAlpha);
		}
		if (cur_desc.m_ColorWriteMasks[0] != m_Desc.m_ColorWriteMasks[0])
		{
			glColorMask((m_Desc.m_ColorWriteMasks[0] & CMASK_Red) != 0,
					(m_Desc.m_ColorWriteMasks[0] & CMASK_Green) != 0,
					(m_Desc.m_ColorWriteMasks[0] & CMASK_Blue) != 0,
					(m_Desc.m_ColorWriteMasks[0] & CMASK_Alpha) != 0);
		}

		if (&cur_blend_factor != &cpBlendFactor)
		{
			glBlendColor(cpBlendFactor.x, cpBlendFactor.y, cpBlendFactor.z, cpBlendFactor.w);
		}
	}

	void OGLESBlendStateObj::ForceDefaultState()
	{
		BlendStateDesc desc;

		if (desc.m_bAlphaToCoverageEnable)
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		if (desc.m_BlendEnables[0])
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
		glBlendEquationSeparate(OGLESMapping::Mapping(desc.m_BlendOps[0]), OGLESMapping::Mapping(desc.m_BlendOpAlphas[0]));
		glBlendFuncSeparate(OGLESMapping::Mapping(desc.m_SrcBlends[0]), OGLESMapping::Mapping(desc.m_DestBlends[0]),
				OGLESMapping::Mapping(desc.m_SrcBlendAlphas[0]), OGLESMapping::Mapping(desc.m_DestBlendAlphas[0]));
		glColorMask((desc.m_ColorWriteMasks[0] & CMASK_Red) != 0,
				(desc.m_ColorWriteMasks[0] & CMASK_Green) != 0,
				(desc.m_ColorWriteMasks[0] & CMASK_Blue) != 0,
				(desc.m_ColorWriteMasks[0] & CMASK_Alpha) != 0);

		glBlendColor(1, 1, 1, 1);
	}


	OGLESSamplerStateObj::OGLESSamplerStateObj(SamplerStateDesc const & cpDesc)
		: CSamplerStateObj(cpDesc),
			m_eGlAddrModeU(OGLESMapping::Mapping(m_Desc.m_eAddrModeU)),
			m_eGlAddrModeV(OGLESMapping::Mapping(m_Desc.m_eAddrModeV)),
			m_eGlAddrModeW(OGLESMapping::Mapping(m_Desc.m_eAddrModeW))
	{
		// AMD's GLES2 emulator don't support MIPMAP well
		if (m_Desc.m_eFilter & TFOE_Min_Linear)
		{
			m_eGlMinFilter = GL_LINEAR;
			/*if (m_Desc.filter & TFOE_Mip_Linear)
			{
				ogl_min_filter_ = GL_LINEAR_MIPMAP_LINEAR;
			}
			else
			{
				ogl_min_filter_ = GL_LINEAR_MIPMAP_NEAREST;
			}*/
		}
		else
		{
			m_eGlMinFilter = GL_NEAREST;
			/*if (m_Desc.filter & TFOE_Mip_Linear)
			{
				ogl_min_filter_ = GL_NEAREST_MIPMAP_LINEAR;
			}
			else
			{
				ogl_min_filter_ = GL_NEAREST_MIPMAP_NEAREST;
			}*/
		}
		if (m_Desc.m_eFilter & TFOE_Mag_Linear)
		{
			m_eGlMagFilter = GL_LINEAR;
		}
		else
		{
			m_eGlMagFilter = GL_NEAREST;
		}
		if (m_Desc.m_eFilter & TFOE_Anisotropic)
		{
			m_eGlMagFilter = GL_LINEAR;
			m_eGlMagFilter = GL_LINEAR;
			//ogl_min_filter_ = GL_LINEAR_MIPMAP_LINEAR;
		}
	}
	//不懂
	void OGLESSamplerStateObj::Active(uint32 uStage, CTexturePtr const & cpTexture)
	{
		OGLESTexture& tex = *checked_pointer_cast<OGLESTexture>(cpTexture);

		tex.TexParameteri(GL_TEXTURE_WRAP_S, m_eGlAddrModeU);
		tex.TexParameteri(GL_TEXTURE_WRAP_T, m_eGlAddrModeV);
		//如果支持就使用
		/*if (glloader_GLES_OES_texture_3D())
		{
			tex.TexParameteri(GL_TEXTURE_WRAP_R_OES, m_eGlAddrModeW);
		}*/

		tex.TexParameteri(GL_TEXTURE_MAG_FILTER, m_eGlMagFilter);
		tex.TexParameteri(GL_TEXTURE_MIN_FILTER, m_eGlMinFilter);

	/*	if (glloader_GLES_EXT_texture_filter_anisotropic())
		{*/
			if (m_Desc.m_eFilter & TFOE_Anisotropic)
			{
				tex.TexParameteri(GL_TEXTURE_MAX_ANISOTROPY_EXT, m_Desc.m_uMaxAnisotropy);
			}
			else
			{
				tex.TexParameteri(GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
			}
	/*	}*/
	}
}
