#include "render.h"
#include "data.h"
#include <SE3DEngine/DeviceLayer/WindApp.h>
#include <SE3DEngine/DeviceLayer/DeviceHelp.h>

void main(int argc, char *argv[])
{
	SE::CreateWindowApp(512,512, 10, 10, "test");

	// ��OpenGL��DirectX��l��
	if ( SE::InitGraphicsDevice())
	{
		exit(0);
	}
	g_view_matrix.Identity();
	if ( !InitResourceOpenGL() )
	{
		ReleaseResourceOpenGL();
		printf("Failed to load resources\n");
		exit(0);
	}

	// �D�j��
	while( SE::ProcessMessageSE() )
	{
		
		RenderFrameOpenGL();
	}
	
	// ����shader
	ReleaseResourceOpenGL();

	// ����OpenGL/DirectXø�ϸ˸m
	SE::ReleaseGraphicsDevice();

}