#include <stdio.h>
#include <stdlib.h>

#include <SEMath/Quaternion.h>

Quaternion g_Quaternion_Zero = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
Quaternion g_Quaternion_One = Quaternion(1.0f, 1.0f, 1.0f, 1.0f);
Quaternion g_Quaternion_Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
//Quaternion g_VectorCPU_MinusOne = Quaternion(-1.0f, -1.0f, -1.0f, -1.0f);
//Quaternion g_VectorCPU_AbsMask = Quaternion( (unsigned int)0x7fffffff );
//Quaternion g_VectorCPU_SignMask = Quaternion( (unsigned int)0x80000000 );

void Quaternion::ConsoleOutput(void)
{
	printf("Vector: (%5.2f,%5.2f,%5.2f,%5.2f)\n", x, y, z, w);
}
