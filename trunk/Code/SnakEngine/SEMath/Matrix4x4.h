#ifndef _MATRIX4X4_
#define _MATRIX4X4_

#include <SEMath/Vector4.h>

// General CPU Version, reference code
#include <SEMath/Matrix4x4_Reference.h> 
// Intel SSE instruction version
#include <SEMath/Matrix4x4_SSE.h> 
#include <SE3DEngine/Config/Config.h>


#ifndef SE_CPU_X86
typedef Matrix4x4CPU Matrix4x4;
#else
typedef Matrix4x4SSE Matrix4x4;
#endif


#endif