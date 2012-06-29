//stateobj 在这初始化
#ifndef _OGLESRender_H_
#define _OGLESRender_H_

#include <map>
#include <GLES2/gl2.h>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/CRender.h>



namespace SE
{
	class OGLESRender : public CRender
	{
	public:
		OGLESRender();
		~OGLESRender();
		void BindBuffer(GLenum target, GLuint buffer);
	    void DeleteBuffer(GLsizei n, GLuint const * cpBufs);
		void StartRendering();

		void BeginFrame();
		void BeginPass();
		void EndPass();
		void EndFrame();
				// Just for debug or profile propose
		void ForceFlush();
		bool GetFullScreen() const;
		void SetFullScreen(bool bFull);
		
	private:
		std::map<GLenum, GLuint> m_BindedBuffers;
		bool m_bFullSc;

	};

}

#endif
