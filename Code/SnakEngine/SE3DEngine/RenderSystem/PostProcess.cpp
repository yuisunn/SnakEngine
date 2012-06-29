#include "PostProcess.h"


namespace SE
{
		static bool PostProcess::CreateRenderTarget()
		{

		}
		static void ShBeginPass();
		static void ShEndPass();

		static void shSetParamVS();
		static void shSetParamPS();

		static void DrawFullScreenQuad();

		  // Draws screen aligned quad
		static void DrawScreenQuad();
       
		static void ClearScreen(float r, float g, float b, float a);
		static void SetTexture();
}