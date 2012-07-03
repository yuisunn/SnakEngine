#ifndef _Platform_H_
#define _Platform_H_

#define COMPILER_MSVC
//#define COMPILER_GCC

#define SE_WINDOWS
//#define SE_LINUX
//#define SE_ANDROID

#define SE_CPU_X86  
//#define SE_CPU_X64
//#define SE_CPU_PPC
//#define SE_CPU_ARM
#define SE_D3D11
//#define SE_GL
//#define SE_GLES

    //内存结构
	// Defines the native endian
	#if defined(SE_CPU_PPC) || defined(SE_CPU_ARM)
		#define SE_BIG_ENDIAN
	#elif defined(SE_CPU_X86) || defined(SE_CPU_X64) || defined(SE_WINDOWS)
		#define SE_LITTLE_ENDIAN
	#else
		#define SE_BIG_ENDIAN
	#endif
    //图形api
#if defined(SE_DIRECT3D9)
  #define SETexture LPDIRECT3DTEXTURE9
  #define SEDevice LPDIRECT3DDEVICE9
#endif


//stl支持
#define SELECT1ST2ND_SUPPORT

#endif
