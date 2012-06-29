#ifndef _DeviceHelp_H_
#define _DeviceHelp_H_
#include <glew.h>
#include <wglew.h>
#include <gl/GL.h>
#define SE_GL
namespace SE
{
  struct SDeviceDesc
  {
	int m_iX, m_iY; // position
	int m_iWidth, m_iHeight; // size
	int m_iMultiSamples;
	bool m_bFullScreen; // fullscreen or windowed
	char *m_cDevice;

	SDeviceDesc(void)
	{
		m_iX = m_iY = 256;
		m_iWidth = m_iHeight = 512;
		m_iMultiSamples = 0;
		m_bFullScreen = false;
		m_cDevice = NULL;
	}
  };
#ifdef SE_GL
  static HDC g_hDC = NULL;
  static HGLRC g_hGLRC = NULL;
   // make my own glProgramLocalParameters4fvEXT
  void APIENTRY _my_glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat* params);
  void FixupGLExt(void);
  bool SetPixelformat(void);
  
  bool InitGraphicsDevice(SDeviceDesc *pDesc=NULL);
  void SwapBuffers(void);
  bool ReleaseGraphicsDevice(void);
#endif

	
}

#endif
