#include "render.h"
#include "data.h"
#include <SE3DEngine/DeviceLayer/WindApp.h>
#include <SE3DEngine/DeviceLayer/DeviceHelp.h>

void main(int argc, char *argv[])
{
	SE::CreateWindowApp(512,512, 10, 10, "test");

	// 做OpenGL或DirectX初始化
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

	// 主迴圈
	while( SE::ProcessMessageSE() )
	{
		
		RenderFrameOpenGL();
	}
	
	// 卸載shader
	ReleaseResourceOpenGL();

	// 關閉OpenGL/DirectX繪圖裝置
	SE::ReleaseGraphicsDevice();

}