#include <SE3DEngine/DeviceLayer/WindApp.h>
#include <SE3DEngine/DeviceLayer/DeviceHelp.h>


namespace SE
{
	//gldevice ��ʼ�� ���ͷ�
#ifdef SE_GL
   // make my own glProgramLocalParameters4fvEXT
void APIENTRY _my_glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat* params)
{
	for ( int i=0; i<count; i++ )
	{
		glProgramLocalParameter4fvARB(target, index+i, params + 4*i);
	}
}
    void FixupGLExt(void)
    {
	if ( glProgramLocalParameters4fvEXT==NULL && glProgramLocalParameter4fvARB )
	{
		glProgramLocalParameters4fvEXT = _my_glProgramLocalParameters4fvEXT;
	}
	}
   bool SetPixelformat(void)
   {
	   HWND hWnd = SE::GetWindowHandle();
	   g_hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 24; // 24 bits zbuffer
	pfd.cStencilBits = 8; // 8 bits stencil buffer
	pfd.iLayerType = PFD_MAIN_PLANE; // main layer

	int pixelformat = ChoosePixelFormat(g_hDC, &pfd);

    if ( pixelformat == 0 ) 
    { 
        return false; 
    } 

    if ( SetPixelFormat(g_hDC, pixelformat, &pfd) == FALSE) 
    { 
		ReleaseDC(hWnd, g_hDC);
        return false; 
    }

	g_hGLRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hGLRC);

    return true; 
}
	bool InitGraphicsDevice(SDeviceDesc *pDesc)
	{
		int multisamples = 0;

	if ( pDesc )
		multisamples = pDesc->m_iMultiSamples;
	
	// `�}�ҵ����ɴN�w���o�o�ӥΨӥN�����������, �������Өϥ�.`
	HWND hWnd = SE::GetWindowHandle();
	if ( hWnd==NULL )
		return false;

	if ( multisamples )
	{
		if ( !SetPixelformat() )
		{
			return false;
		}
	}
	else
	{
		if ( !SetPixelformat() )
		{
			return false;
		}
	}

	glewInit();
	FixupGLExt();

	return true;
	}
	void SwapBuffers(void)
	{
	// `�e�{�X�I��`
	SwapBuffers(g_hDC);
}
		bool ReleaseGraphicsDevice(void)
{
	HWND hWnd = SE::GetWindowHandle();

	wglMakeCurrent(g_hDC, NULL);
	wglDeleteContext(g_hGLRC);
	g_hGLRC = NULL;

	ReleaseDC(hWnd, g_hDC);
	g_hDC = NULL;

	return true;
}
#endif
	
}
