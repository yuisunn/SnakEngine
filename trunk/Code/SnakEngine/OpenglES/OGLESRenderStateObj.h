#ifndef _OGLESRenderStateObj_H_
#define _OGLESRenderStateObj_H_



#include <GLES2/gl2.h>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/CRenderStateObj.h>

namespace SE
{
    class OGLESRasterizerStateObj : public CRasterizerStateObj
	{
	public:
		explicit OGLESRasterizerStateObj(RasterizerStateDesc const & cpDesc);

		void Active();
		void ForceDefaultState();

	private:
		GLenum m_eGlFrontFace;
	};

	class OGLESDepthStencilStateObj : public CDepthStencilStateObj
	{
	public:
		explicit OGLESDepthStencilStateObj(DepthStencilStateDesc const & cpDesc);

		void Active(uint16 uFrontStencilRef, uint16 uBackStencilRef);
		void ForceDefaultState();

	private:
		GLboolean m_bGlDepthWriteMask;
		GLenum m_eGlDepthFunc;
		GLenum m_eGlFrontStencilFunc;
		GLenum m_eGlFrontStencilFail;
		GLenum m_eGlFrontStencilDepthFail;
		GLenum m_eGlFrontStencilPass;
		GLenum m_eGlBackStencilFunc;
		GLenum m_eGlBackStencilFail;
		GLenum m_eGlBackStencilDepthFail;
		GLenum m_eGlBackStencilPass;
	};

	class OGLESBlendStateObj : public CBlendStateObj
	{
	public:
		explicit OGLESBlendStateObj(BlendStateDesc const & cpDesc);

		void Active(Color const & cpBlendFactor, uint32 uSampleMask);
		void ForceDefaultState();

	private:
		GLenum m_eGlBlendOp;
		GLenum m_eGlBlendOpAlpha;
		GLenum m_eGlSrcBlend;
		GLenum m_eGlDestBlend;
		GLenum m_eGlSrcBlendAlpha;
		GLenum m_eGlDestBlendAlpha;
	};

	class OGLESSamplerStateObj : public CSamplerStateObj
	{
	public:
		explicit OGLESSamplerStateObj(SamplerStateDesc const & cpDesc);

		void Active(uint32 uStage, CTexturePtr const & cpTexture);

	private:
		GLenum m_eGlAddrModeU;
		GLenum m_eGlAddrModeV;
		GLenum m_eGlAddrModeW;
		GLenum m_eGlMinFilter;
		GLenum m_eGlMagFilter;
	};
}
#endif 

