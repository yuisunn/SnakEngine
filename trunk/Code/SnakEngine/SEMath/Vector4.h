#ifndef _VECTOR4_
#define _VECTOR4_

// GENERAL CPU Version, reference code.
#include <SEMath/Vector4_Reference.h>
// Intel SSE instruction Version
#include <SEMath/Vector4_SSE.h>
#include <SE3DEngine/Config/Platform.h>

#ifndef SE_CPU_X86
typedef Vector4CPU Vector4;
#else
typedef Vector4SSE Vector4;
#endif

#endif // _VECTOR4_