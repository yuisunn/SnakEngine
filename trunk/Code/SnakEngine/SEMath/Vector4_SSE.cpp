
#include <stdio.h>
#include <stdlib.h>

#include <SEMath/Vector4.h>

Vector4SSE g_Vector4SSE_XYZ_Mask = Vector4SSE(0xffffffff, 0xffffffff, 0xffffffff, 0x00000000);
Vector4SSE g_Vector4SSE_Zero = Vector4SSE(0.0f, 0.0f, 0.0f, 0.0f);
Vector4SSE g_Vector4SSE_One = Vector4SSE(1.0f, 1.0f, 1.0f, 1.0f);
Vector4SSE g_Vector4SSE_MinusOne = Vector4SSE(-1.0f, -1.0f, -1.0f, -1.0f);
Vector4SSE g_Vector4SSE_AbsMask = Vector4SSE( (unsigned int)0x7fffffff );
Vector4SSE g_Vector4SSE_SignMask = Vector4SSE( (unsigned int)0x80000000 );

void Vector4SSE::ConsoleOutput(void)
{
	printf("Vector: (%5.2f,%5.2f,%5.2f,%5.2f)\n", x, y, z, w);
}

