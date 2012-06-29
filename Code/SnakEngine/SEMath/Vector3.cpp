#include <stdio.h>
#include <stdlib.h>

#include <SEMath/Vector3.h>



Vector3 g_Vector3_Zero = Vector3(0.0f, 0.0f, 0.0f);
Vector3 g_Vector3_One = Vector3(1.0f, 1.0f, 1.0f);
Vector3 g_Vector3_MinusOne = Vector3(-1.0f, -1.0f, -1.0f);
Vector3 g_Vector3_AbsMask = Vector3( (unsigned int)0x7fffffff );
Vector3 g_Vector3_SignMask = Vector3( (unsigned int)0x80000000 );

void Vector3::ConsoleOutput(void)
{
	printf("Vector: (%5.2f,%5.2f,%5.2f,%5.2f)\n", x, y, z);
}
