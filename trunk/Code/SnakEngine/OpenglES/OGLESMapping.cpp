
#include <OpenglES/OGLESMapping.h>


namespace SE
{

	void OGLESMapping::Mapping(GLfloat* pGlClr4, Color const & cpClr)
	{
		pGlClr4[0] = cpClr.x;
		pGlClr4[1] = cpClr.y;
		pGlClr4[2] = cpClr.z;
		pGlClr4[3] = cpClr.w;

	}
    GLenum Mapping(ECompareFunction eFunc)
	{
		switch (eFunc)
		{
		case CF_AlwaysFail:
			return GL_NEVER;

		case CF_AlwaysPass:
			return GL_ALWAYS;

		case CF_Less:
			return GL_LESS;

		case CF_LessEqual:
			return GL_LEQUAL;

		case CF_Equal:
			return GL_EQUAL;

		case CF_NotEqual:
			return GL_NOTEQUAL;

		case CF_GreaterEqual:
			return GL_GEQUAL;

		case CF_Greater:
			return GL_GREATER;

		default:
			BOOST_ASSERT(false);
			return GL_EQUAL;
		};


	}

		static GLenum Mapping(EStencilOperation Stencilo);

		static GLenum Mapping(EAlphaBlendFactor Alphaf);
		static GLenum Mapping(EBlendOperation Blendo);
		static GLint Mapping(ETexAddressingMode Mode);
		
		static void Mapping(GLenum& pPrimType, uint32& upPrimCount, RenderLayout const & cpRl);

		static void MappingFormat(GLint& pInternalFormat, GLenum& pGlformat, GLenum& pGltype, PixelFormat ef);
		static void MappingVertexFormat(GLenum& pGltype, GLboolean& pNormalized, PixelFormat ef);
}