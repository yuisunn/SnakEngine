///egine 映射 opengles 插件的一些东西
#ifndef _OGLESMapping_H_
#define _OGLESMapping_H_

#include <GLES2/gl2.h>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/PixelFormat.h>
#include <SE3DEngine/RenderSystem/CRenderStateObj.h>


namespace SE
{
    class OGLESMapping
    {
    public:
	    OGLESMapping(void);
	    ~OGLESMapping(void);
		static void Mapping(GLfloat* fClr4, Color const & cpClr);
		static GLenum Mapping(ECompareFunction Func);

		static GLenum Mapping(EStencilOperation Stencilo);

		static GLenum Mapping(EAlphaBlendFactor Alphaf);
		static GLenum Mapping(EBlendOperation Blendo);
		static GLint Mapping(ETexAddressingMode Mode);

		static void Mapping(GLenum& pPrimType, uint32& upPrimCount, RenderLayout const & cpRl);

		static void MappingFormat(GLint& pInternalFormat, GLenum& pGlformat, GLenum& pGltype, PixelFormat ef);
		static void MappingVertexFormat(GLenum& pGltype, GLboolean& pNormalized, PixelFormat ef);

    };
}

#endif
