// PixelFormat.hpp

#ifndef _PixelFormat_H_
#define _PixelFormat_H_

 

#include <vector>
#include <boost/assert.hpp>
#include <boost/bind.hpp>

#include <SE3DEngine/Comm/ThrowErr.h>

#include <SE3DEngine/Comm/Def.h>


namespace SE
{

	#ifndef DXGI_FORMAT_DEFINED
	enum DXGI_FORMAT
	{
		DXGI_FORMAT_UNKNOWN	                    = 0,
		DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
		DXGI_FORMAT_R32G32B32A32_UINT           = 3,
		DXGI_FORMAT_R32G32B32A32_SINT           = 4,
		DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
		DXGI_FORMAT_R32G32B32_FLOAT             = 6,
		DXGI_FORMAT_R32G32B32_UINT              = 7,
		DXGI_FORMAT_R32G32B32_SINT              = 8,
		DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
		DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
		DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
		DXGI_FORMAT_R16G16B16A16_UINT           = 12,
		DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
		DXGI_FORMAT_R16G16B16A16_SINT           = 14,
		DXGI_FORMAT_R32G32_TYPELESS             = 15,
		DXGI_FORMAT_R32G32_FLOAT                = 16,
		DXGI_FORMAT_R32G32_UINT                 = 17,
		DXGI_FORMAT_R32G32_SINT                 = 18,
		DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
		DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
		DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
		DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
		DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
		DXGI_FORMAT_R10G10B10A2_UINT            = 25,
		DXGI_FORMAT_R11G11B10_FLOAT             = 26,
		DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
		DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
		DXGI_FORMAT_R8G8B8A8_UINT               = 30,
		DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
		DXGI_FORMAT_R8G8B8A8_SINT               = 32,
		DXGI_FORMAT_R16G16_TYPELESS             = 33,
		DXGI_FORMAT_R16G16_FLOAT                = 34,
		DXGI_FORMAT_R16G16_UNORM                = 35,
		DXGI_FORMAT_R16G16_UINT                 = 36,
		DXGI_FORMAT_R16G16_SNORM                = 37,
		DXGI_FORMAT_R16G16_SINT                 = 38,
		DXGI_FORMAT_R32_TYPELESS                = 39,
		DXGI_FORMAT_D32_FLOAT                   = 40,
		DXGI_FORMAT_R32_FLOAT                   = 41,
		DXGI_FORMAT_R32_UINT                    = 42,
		DXGI_FORMAT_R32_SINT                    = 43,
		DXGI_FORMAT_R24G8_TYPELESS              = 44,
		DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
		DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
		DXGI_FORMAT_R8G8_TYPELESS               = 48,
		DXGI_FORMAT_R8G8_UNORM                  = 49,
		DXGI_FORMAT_R8G8_UINT                   = 50,
		DXGI_FORMAT_R8G8_SNORM                  = 51,
		DXGI_FORMAT_R8G8_SINT                   = 52,
		DXGI_FORMAT_R16_TYPELESS                = 53,
		DXGI_FORMAT_R16_FLOAT                   = 54,
		DXGI_FORMAT_D16_UNORM                   = 55,
		DXGI_FORMAT_R16_UNORM                   = 56,
		DXGI_FORMAT_R16_UINT                    = 57,
		DXGI_FORMAT_R16_SNORM                   = 58,
		DXGI_FORMAT_R16_SINT                    = 59,
		DXGI_FORMAT_R8_TYPELESS                 = 60,
		DXGI_FORMAT_R8_UNORM                    = 61,
		DXGI_FORMAT_R8_UINT                     = 62,
		DXGI_FORMAT_R8_SNORM                    = 63,
		DXGI_FORMAT_R8_SINT                     = 64,
		DXGI_FORMAT_A8_UNORM                    = 65,
		DXGI_FORMAT_R1_UNORM                    = 66,
		DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
		DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
		DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
		DXGI_FORMAT_BC1_TYPELESS                = 70,
		DXGI_FORMAT_BC1_UNORM                   = 71,
		DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
		DXGI_FORMAT_BC2_TYPELESS                = 73,
		DXGI_FORMAT_BC2_UNORM                   = 74,
		DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
		DXGI_FORMAT_BC3_TYPELESS                = 76,
		DXGI_FORMAT_BC3_UNORM                   = 77,
		DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
		DXGI_FORMAT_BC4_TYPELESS                = 79,
		DXGI_FORMAT_BC4_UNORM                   = 80,
		DXGI_FORMAT_BC4_SNORM                   = 81,
		DXGI_FORMAT_BC5_TYPELESS                = 82,
		DXGI_FORMAT_BC5_UNORM                   = 83,
		DXGI_FORMAT_BC5_SNORM                   = 84,
		DXGI_FORMAT_B5G6R5_UNORM                = 85,
		DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
		DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
		DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
		DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
		DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
		DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
		DXGI_FORMAT_BC6H_TYPELESS               = 94,
		DXGI_FORMAT_BC6H_UF16                   = 95,
		DXGI_FORMAT_BC6H_SF16                   = 96,
		DXGI_FORMAT_BC7_TYPELESS                = 97,
		DXGI_FORMAT_BC7_UNORM                   = 98,
		DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
		DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
	};
#endif

	

	enum EPixelChannel
	{
		EC_R = 0UL,
		EC_G = 1UL,
		EC_B = 2UL,
		EC_A = 3UL,
		EC_D = 4UL,
		EC_S = 5UL,
		EC_BC = 6UL,
		EC_E = 7UL
	};

	enum EPixelChannelType
	{
		ECT_UNorm = 0UL,
		ECT_SNorm = 1UL,
		ECT_UInt = 2UL,
		ECT_SInt = 3UL,
		ECT_Float = 4UL,
		ECT_UNorm_SRGB = 5UL,
		ECT_Typeless = 6UL,
		ECT_SharedExp = 7UL
	};

	// element format is a 64-bit value:
	// 00000000 T3[4] T2[4] T1[4] T0[4] S3[6] S2[6] S1[6] S0[6] C3[4] C2[4] C1[4] C0[4]

	template <uint64 ch0, uint64 ch1, uint64 ch2, uint64 ch3,
		uint64 ch0_size, uint64 ch1_size, uint64 ch2_size, uint64 ch3_size,
		uint64 ch0_type, uint64 ch1_type, uint64 ch2_type, uint64 ch3_type>
	struct MakePixelFormat4
	{
		static uint64 const value = (ch0 << 0) | (ch1 << 4) | (ch2 << 8) | (ch3 << 12)
			| (ch0_size << 16) | (ch1_size << 22) | (ch2_size << 28) | (ch3_size << 34)
			| (ch0_type << 40) | (ch1_type << 44) | (ch2_type << 48) | (ch3_type << 52);
	};

	template <uint64 ch0, uint64 ch1, uint64 ch2,
		uint64 ch0_size, uint64 ch1_size, uint64 ch2_size,
		uint64 ch0_type, uint64 ch1_type, uint64 ch2_type>
	struct MakePixelFormat3
	{
		static uint64 const value = MakePixelFormat4<ch0, ch1, ch2, 0, ch0_size, ch1_size, ch2_size, 0, ch0_type, ch1_type, ch2_type, 0>::value;
	};

	template <uint64 ch0, uint64 ch1,
		uint64 ch0_size, uint64 ch1_size,
		uint64 ch0_type, uint64 ch1_type>
	struct MakePixelFormat2
	{
		static uint64 const value = MakePixelFormat3<ch0, ch1, 0, ch0_size, ch1_size, 0, ch0_type, ch1_type, 0>::value;
	};

	template <uint64 ch0,
		uint64 ch0_size,
		uint64 ch0_type>
	struct MakePixelFormat1
	{
		static uint64 const value = MakePixelFormat2<ch0, 0, ch0_size, 0, ch0_type, 0>::value;
	};

	typedef uint64 PixelFormat;

	// Unknown element format.
	PixelFormat const EF_Unknown = 0;

	// 8-bit element format, all bits alpha.
	PixelFormat const EF_A8 = MakePixelFormat1<EC_A, 8, ECT_UNorm>::value;

	// 16-bit element format, 4 bits for alpha, red, green and blue.
	PixelFormat const EF_ARGB4 = MakePixelFormat4<EC_A, EC_R, EC_G, EC_B, 4, 4, 4, 4, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 8-bit element format, 8 bits for red.
	PixelFormat const EF_R8 = MakePixelFormat1<EC_R, 8, ECT_UNorm>::value;
	// 8-bit element format, 8 bits for signed red.
	PixelFormat const EF_SIGNED_R8 = MakePixelFormat1<EC_R, 8, ECT_SNorm>::value;
	// 16-bit element format, 8 bits for red, green.
	PixelFormat const EF_GR8 = MakePixelFormat2<EC_G, EC_R, 8, 8, ECT_UNorm, ECT_UNorm>::value;
	// 16-bit element format, 8 bits for signed red, green.
	PixelFormat const EF_SIGNED_GR8 = MakePixelFormat2<EC_G, EC_R, 8, 8, ECT_SNorm, ECT_SNorm>::value;
	// 24-bit element format, 8 bits for red, green and blue.
	PixelFormat const EF_BGR8 = MakePixelFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 24-bit element format, 8 bits for signed red, green and blue.
	PixelFormat const EF_SIGNED_BGR8 = MakePixelFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;
	// 32-bit element format, 8 bits for alpha, red, green and blue.
	PixelFormat const EF_ARGB8 = MakePixelFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 32-bit element format, 8 bits for alpha, red, green and blue.
	PixelFormat const EF_ABGR8 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 32-bit element format, 8 bits for signed alpha, red, green and blue.
	PixelFormat const EF_SIGNED_ABGR8 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;
	// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
	PixelFormat const EF_A2BGR10 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 32-bit element format, 2 bits for alpha, 10 bits for signed red, green and blue.
	PixelFormat const EF_SIGNED_A2BGR10 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue.
	PixelFormat const EF_R8UI = MakePixelFormat1<EC_R, 8, ECT_UInt>::value;
	// 32-bit element format, 8 bits for alpha, red, green and blue.
	PixelFormat const EF_R8I = MakePixelFormat1<EC_R, 8, ECT_SInt>::value;
	// 16-bit element format, 8 bits for red, green.
	PixelFormat const EF_GR8UI = MakePixelFormat2<EC_G, EC_R, 8, 8, ECT_UInt, ECT_UInt>::value;
	// 16-bit element format, 8 bits for red, green.
	PixelFormat const EF_GR8I = MakePixelFormat2<EC_G, EC_R, 8, 8, ECT_SInt, ECT_SInt>::value;
	// 24-bit element format, 8 bits for red, green and blue.
	PixelFormat const EF_BGR8UI = MakePixelFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 24-bit element format, 8 bits for red, green and blue.
	PixelFormat const EF_BGR8I = MakePixelFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt>::value;
	// 32-bit element format, 8 bits for alpha, red, green and blue.
	PixelFormat const EF_ABGR8UI = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 32-bit element format, 8 bits for signed alpha, red, green and blue.
	PixelFormat const EF_ABGR8I = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;
	// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
	PixelFormat const EF_A2BGR10UI = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
	PixelFormat const EF_A2BGR10I = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 16-bit element format, 16 bits for red.
	PixelFormat const EF_R16 = MakePixelFormat1<EC_R, 16, ECT_UNorm>::value;
	// 16-bit element format, 16 bits for signed red.
	PixelFormat const EF_SIGNED_R16 = MakePixelFormat1<EC_R, 16, ECT_SNorm>::value;
	// 32-bit element format, 16 bits for red and green.
	PixelFormat const EF_GR16 = MakePixelFormat2<EC_G, EC_R, 16, 16, ECT_UNorm, ECT_UNorm>::value;
	// 32-bit element format, 16 bits for signed red and green.
	PixelFormat const EF_SIGNED_GR16 = MakePixelFormat2<EC_G, EC_R, 16, 16, ECT_SNorm, ECT_SNorm>::value;
	// 48-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_BGR16 = MakePixelFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 48-bit element format, 16 bits for signed alpha, blue, green and red.
	PixelFormat const EF_SIGNED_BGR16 = MakePixelFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;
	// 64-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_ABGR16 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 64-bit element format, 16 bits for signed alpha, blue, green and red.
	PixelFormat const EF_SIGNED_ABGR16 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;
	// 32-bit element format, 32 bits for red.
	PixelFormat const EF_R32 = MakePixelFormat1<EC_R, 32, ECT_UNorm>::value;
	// 32-bit element format, 32 bits for signed red.
	PixelFormat const EF_SIGNED_R32 = MakePixelFormat1<EC_R, 32, ECT_SNorm>::value;
	// 64-bit element format, 16 bits for red and green.
	PixelFormat const EF_GR32 = MakePixelFormat2<EC_G, EC_R, 32, 32, ECT_UNorm, ECT_UNorm>::value;
	// 64-bit element format, 16 bits for signed red and green.
	PixelFormat const EF_SIGNED_GR32 = MakePixelFormat2<EC_G, EC_R, 32, 32, ECT_SNorm, ECT_SNorm>::value;
	// 96-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_BGR32 = MakePixelFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
	PixelFormat const EF_SIGNED_BGR32 = MakePixelFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;
	// 128-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_ABGR32 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;
	// 128-bit element format, 16 bits for signed alpha, blue, green and red.
	PixelFormat const EF_SIGNED_ABGR32 = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 16-bit element format, 16 bits for red.
	PixelFormat const EF_R16UI = MakePixelFormat1<EC_R, 16, ECT_UInt>::value;
	// 16-bit element format, 16 bits for signed red.
	PixelFormat const EF_R16I = MakePixelFormat1<EC_R, 16, ECT_SInt>::value;
	// 32-bit element format, 16 bits for red and green.
	PixelFormat const EF_GR16UI = MakePixelFormat2<EC_G, EC_R, 16, 16, ECT_UInt, ECT_UInt>::value;
	// 32-bit element format, 16 bits for signed red and green.
	PixelFormat const EF_GR16I = MakePixelFormat2<EC_G, EC_R, 16, 16, ECT_SInt, ECT_SInt>::value;
	// 48-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_BGR16UI = MakePixelFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 48-bit element format, 16 bits for signed alpha, blue, green and red.
	PixelFormat const EF_BGR16I = MakePixelFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt>::value;
	// 64-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_ABGR16UI = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 64-bit element format, 16 bits for signed alpha, blue, green and red.
	PixelFormat const EF_ABGR16I = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;
	// 32-bit element format, 32 bits for red.
	PixelFormat const EF_R32UI = MakePixelFormat1<EC_R, 32, ECT_UInt>::value;
	// 32-bit element format, 32 bits for signed red.
	PixelFormat const EF_R32I = MakePixelFormat1<EC_R, 32, ECT_SInt>::value;
	// 64-bit element format, 16 bits for red and green.
	PixelFormat const EF_GR32UI = MakePixelFormat2<EC_G, EC_R, 32, 32, ECT_UInt, ECT_UInt>::value;
	// 64-bit element format, 16 bits for signed red and green.
	PixelFormat const EF_GR32I = MakePixelFormat2<EC_G, EC_R, 32, 32, ECT_SInt, ECT_SInt>::value;
	// 96-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_BGR32UI = MakePixelFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
	PixelFormat const EF_BGR32I = MakePixelFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt>::value;
	// 128-bit element format, 16 bits for alpha, blue, green and red.
	PixelFormat const EF_ABGR32UI = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;
	// 128-bit element format, 16 bits for signed alpha, blue, green and red.
	PixelFormat const EF_ABGR32I = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 16-bit element format, 16 bits floating-point for red.
	PixelFormat const EF_R16F = MakePixelFormat1<EC_R, 16, ECT_Float>::value;
	// 32-bit element format, 16 bits floating-point for green and red.
	PixelFormat const EF_GR16F = MakePixelFormat2<EC_G, EC_R, 16, 16, ECT_Float, ECT_Float>::value;
	// 32-bit element format, 11 bits floating-point for green and red, 10 bits floating-point for blue.
	PixelFormat const EF_B10G11R11F = MakePixelFormat3<EC_B, EC_G, EC_R, 10, 11, 11, ECT_Float, ECT_Float, ECT_Float>::value;
	// 48-bit element format, 16 bits floating-point for blue, green and red.
	PixelFormat const EF_BGR16F = MakePixelFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float>::value;
	// 64-bit element format, 16 bits floating-point for alpha, blue, green and red.
	PixelFormat const EF_ABGR16F = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value;
	// 32-bit element format, 32 bits floating-point for red.
	PixelFormat const EF_R32F = MakePixelFormat1<EC_R, 32, ECT_Float>::value;
	// 64-bit element format, 32 bits floating-point for green and red.
	PixelFormat const EF_GR32F = MakePixelFormat2<EC_G, EC_R, 32, 32, ECT_Float, ECT_Float>::value;
	// 96-bit element format, 32 bits floating-point for blue, green and red.
	PixelFormat const EF_BGR32F = MakePixelFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float>::value;
	// 128-bit element format, 32 bits floating-point for alpha, blue, green and red.
	PixelFormat const EF_ABGR32F = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value;

	// BC1 compression element format, DXT1
	PixelFormat const EF_BC1 = MakePixelFormat1<EC_BC, 1, ECT_UNorm>::value;
	// BC1 compression element format, signed DXT1
	PixelFormat const EF_SIGNED_BC1 = MakePixelFormat1<EC_BC, 1, ECT_SNorm>::value;
	// BC2 compression element format, DXT3
	PixelFormat const EF_BC2 = MakePixelFormat1<EC_BC, 2, ECT_UNorm>::value;
	// BC2 compression element format, signed DXT3
	PixelFormat const EF_SIGNED_BC2 = MakePixelFormat1<EC_BC, 2, ECT_SNorm>::value;
	// BC3 compression element format, DXT5
	PixelFormat const EF_BC3 = MakePixelFormat1<EC_BC, 3, ECT_UNorm>::value;
	// BC3 compression element format, signed DXT5
	PixelFormat const EF_SIGNED_BC3 = MakePixelFormat1<EC_BC, 3, ECT_SNorm>::value;
	// BC4 compression element format, 1 channel
	PixelFormat const EF_BC4 = MakePixelFormat1<EC_BC, 4, ECT_UNorm>::value;
	// BC4 compression element format, 1 channel signed
	PixelFormat const EF_SIGNED_BC4 = MakePixelFormat1<EC_BC, 4, ECT_SNorm>::value;
	// BC5 compression element format, 2 channels
	PixelFormat const EF_BC5 = MakePixelFormat1<EC_BC, 5, ECT_UNorm>::value;
	// BC5 compression element format, 2 channels signed
	PixelFormat const EF_SIGNED_BC5 = MakePixelFormat1<EC_BC, 5, ECT_SNorm>::value;
	// BC6 compression element format, 3 channels
	PixelFormat const EF_BC6 = MakePixelFormat1<EC_BC, 6, ECT_UNorm>::value;
	// BC6 compression element format, 3 channels
	PixelFormat const EF_SIGNED_BC6 = MakePixelFormat1<EC_BC, 6, ECT_SNorm>::value;
	// BC7 compression element format, 3 channels
	PixelFormat const EF_BC7 = MakePixelFormat1<EC_BC, 7, ECT_UNorm>::value;

	// 16-bit element format, 16 bits depth
	PixelFormat const EF_D16 = MakePixelFormat1<EC_D, 16, ECT_UNorm>::value;
	// 32-bit element format, 24 bits depth and 8 bits stencil
	PixelFormat const EF_D24S8 = MakePixelFormat2<EC_D, EC_S, 24, 8, ECT_UNorm, ECT_UInt>::value;
	// 32-bit element format, 32 bits depth
	PixelFormat const EF_D32F = MakePixelFormat1<EC_D, 32, ECT_Float>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
	PixelFormat const EF_ARGB8_SRGB = MakePixelFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value;
	// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
	PixelFormat const EF_ABGR8_SRGB = MakePixelFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value;
	// BC1 compression element format. Standard RGB (gamma = 2.2).
	PixelFormat const EF_BC1_SRGB = MakePixelFormat1<EC_BC, 1, ECT_UNorm_SRGB>::value;
	// BC2 compression element format. Standard RGB (gamma = 2.2).
	PixelFormat const EF_BC2_SRGB = MakePixelFormat1<EC_BC, 2, ECT_UNorm_SRGB>::value;
	// BC3 compression element format. Standard RGB (gamma = 2.2).
	PixelFormat const EF_BC3_SRGB = MakePixelFormat1<EC_BC, 3, ECT_UNorm_SRGB>::value;
	// BC4 compression element format. Standard RGB (gamma = 2.2).
	PixelFormat const EF_BC4_SRGB = MakePixelFormat1<EC_BC, 4, ECT_UNorm_SRGB>::value;
	// BC5 compression element format. Standard RGB (gamma = 2.2).
	PixelFormat const EF_BC5_SRGB = MakePixelFormat1<EC_BC, 5, ECT_UNorm_SRGB>::value;
	// BC7 compression element format. Standard RGB (gamma = 2.2).
	PixelFormat const EF_BC7_SRGB = MakePixelFormat1<EC_BC, 7, ECT_UNorm_SRGB>::value;

	PixelFormat FromDXGIFormat(uint32 format);
	

	template <int c>
	inline EPixelChannel Channel(PixelFormat ef)
	{
		return static_cast<EPixelChannel>((ef >> (4 * c)) & 0xF);
	}

	template <int c>
	inline PixelFormat Channel(PixelFormat ef, EPixelChannel new_c)
	{
		ef &= ~(0xFULL << (4 * c));
		ef |= (static_cast<uint64>(new_c) << (4 * c));
		return ef;
	}

	template <int c>
	inline uint8 ChannelBits(PixelFormat ef)
	{
		return (ef >> (16 + 6 * c)) & 0x3F;
	}

	template <int c>
	inline PixelFormat ChannelBits(PixelFormat ef, uint64 new_c)
	{
		ef &= ~(0x3FULL << (16 + 6 * c));
		ef |= (new_c << (16 + 6 * c));
		return ef;
	}

	template <int c>
	inline EPixelChannelType ChannelType(PixelFormat ef)
	{
		return static_cast<EPixelChannelType>((ef >> (40 + 4 * c)) & 0xF);
	}

	template <int c>
	inline PixelFormat ChannelType(PixelFormat ef, EPixelChannelType new_c)
	{
		ef &= ~(0xFULL << (40 + 4 * c));
		ef |= (static_cast<uint64>(new_c) << (40 + 4 * c));
		return ef;
	}

	inline bool IsFloatFormat(PixelFormat format)
	{
		return (ECT_Float == ChannelType<0>(format));
	}

	inline bool IsCompressedFormat(PixelFormat format)
	{
		return (EC_BC == Channel<0>(format));
	}

	inline bool IsDepthFormat(PixelFormat format)
	{
		return (EC_D == Channel<0>(format));
	}

	inline bool IsStencilFormat(PixelFormat format)
	{
		return (EC_S == Channel<1>(format));
	}

	inline bool IsSRGB(PixelFormat format)
	{
		return (ECT_UNorm_SRGB == ChannelType<0>(format));
	}

	inline bool
	IsSigned(PixelFormat format)
	{
		return (ECT_SNorm == ChannelType<0>(format));
	}

	inline uint8 NumFormatBits(PixelFormat format)
	{
		if (IsCompressedFormat(format))
		{
			switch (ChannelBits<0>(format))
			{
			case 1:
			case 4:
				return 16;

			case 2:
			case 3:
			case 5:
				return 32;

			default:
				BOOST_ASSERT(false);
				return 0;
			}
		}
		else
		{
			return ChannelBits<0>(format) + ChannelBits<1>(format) + ChannelBits<2>(format) + ChannelBits<3>(format);
		}
	}

	inline uint8 NumFormatBytes(PixelFormat format)
	{
		return NumFormatBits(format) / 8;
	}

	inline PixelFormat MakeSRGB(PixelFormat format)
	{
		if (ECT_UNorm == ChannelType<0>(format))
		{
			format = ChannelType<0>(format, ECT_UNorm_SRGB);
		}
		if (!IsCompressedFormat(format))
		{
			if (ECT_UNorm == ChannelType<1>(format))
			{
				format = ChannelType<1>(format, ECT_UNorm_SRGB);
			}
			if (ECT_UNorm == ChannelType<2>(format))
			{
				format = ChannelType<2>(format, ECT_UNorm_SRGB);
			}
			if (ECT_UNorm == ChannelType<3>(format))
			{
				format = ChannelType<3>(format, ECT_UNorm_SRGB);
			}
		}

		return format;
	}

	inline PixelFormat MakeNonSRGB(PixelFormat format)
	{
		if (ECT_UNorm_SRGB == ChannelType<0>(format))
		{
			format = ChannelType<0>(format, ECT_UNorm);
		}
		if (ECT_UNorm_SRGB == ChannelType<1>(format))
		{
			format = ChannelType<1>(format, ECT_UNorm);
		}
		if (ECT_UNorm_SRGB == ChannelType<2>(format))
		{
			format = ChannelType<2>(format, ECT_UNorm);
		}
		if (ECT_UNorm_SRGB == ChannelType<3>(format))
		{
			format = ChannelType<3>(format, ECT_UNorm);
		}

		return format;
	}

	inline PixelFormat MakeSigned(PixelFormat format)
	{
		if (ECT_UNorm == ChannelType<0>(format))
		{
			format = ChannelType<0>(format, ECT_SNorm);
		}
		if (ECT_UNorm == ChannelType<1>(format))
		{
			format = ChannelType<1>(format, ECT_SNorm);
		}
		if (ECT_UNorm == ChannelType<2>(format))
		{
			format = ChannelType<2>(format, ECT_SNorm);
		}
		if (ECT_UNorm == ChannelType<3>(format))
		{
			format = ChannelType<3>(format, ECT_SNorm);
		}

		if (ECT_UInt == ChannelType<0>(format))
		{
			format = ChannelType<0>(format, ECT_SInt);
		}
		if (ECT_UInt == ChannelType<1>(format))
		{
			format = ChannelType<1>(format, ECT_SInt);
		}
		if (ECT_UInt == ChannelType<2>(format))
		{
			format = ChannelType<2>(format, ECT_SInt);
		}
		if (ECT_UInt == ChannelType<3>(format))
		{
			format = ChannelType<3>(format, ECT_SInt);
		}

		return format;
	}

	inline PixelFormat MakeUnsigned(PixelFormat format)
	{
		if (ECT_SNorm == ChannelType<0>(format))
		{
			format = ChannelType<0>(format, ECT_UNorm);
		}
		if (ECT_SNorm == ChannelType<1>(format))
		{
			format = ChannelType<1>(format, ECT_UNorm);
		}
		if (ECT_SNorm == ChannelType<2>(format))
		{
			format = ChannelType<2>(format, ECT_UNorm);
		}
		if (ECT_SNorm == ChannelType<3>(format))
		{
			format = ChannelType<3>(format, ECT_UNorm);
		}

		if (ECT_SInt == ChannelType<0>(format))
		{
			format = ChannelType<0>(format, ECT_UInt);
		}
		if (ECT_SInt == ChannelType<1>(format))
		{
			format = ChannelType<1>(format, ECT_UInt);
		}
		if (ECT_SInt == ChannelType<2>(format))
		{
			format = ChannelType<2>(format, ECT_UInt);
		}
		if (ECT_SInt == ChannelType<3>(format))
		{
			format = ChannelType<3>(format, ECT_UInt);
		}

		return format;
	}

	inline uint8 NumDepthBits(PixelFormat format)
	{
		if (EC_D == Channel<0>(format))
		{
			return ChannelBits<0>(format);
		}
		else
		{
			return 0;
		}
	}

	inline uint8
	NumStencilBits(PixelFormat format)
	{
		if (EC_S == Channel<1>(format))
		{
			return ChannelBits<1>(format);
		}
		else
		{
			return 0;
		}
	}

	inline uint32 NumComponents(PixelFormat format)
	{
		if (IsCompressedFormat(format))
		{
			switch (ChannelBits<0>(format))
			{
			case 1:
			case 2:
			case 3:
				return 4;

			case 4:
				return 1;

			case 5:
				return 2;

			default:
				BOOST_ASSERT(false);
				return 0;
			}
		}
		else
		{
			return (ChannelBits<0>(format) != 0) + (ChannelBits<1>(format) != 0)
				+ (ChannelBits<2>(format) != 0) + (ChannelBits<3>(format) != 0);
		}
	}

	//inline uint32 ComponentBpps(PixelFormat format)
	//{
	//	return std::max(std::max(ChannelBits<0>(format), ChannelBits<1>(format)),
	//		std::max(ChannelBits<2>(format), ChannelBits<3>(format)));
	//}


	enum PixelAccessHint
	{
		EAH_CPU_Read = 1UL << 0,
		EAH_CPU_Write = 1UL << 1,
		EAH_GPU_Read = 1UL << 2,
		EAH_GPU_Write = 1UL << 3,
		EAH_GPU_Unordered = 1UL << 4,
		EAH_GPU_Structured = 1UL << 5,
		EAH_Generate_Mips = 1UL << 6,
		EAH_Immutable = 1UL << 7
	};

	struct PixelInitData
	{
		void const * m_cpData;
		uint32 m_uRowPitch;
		uint32 m_uSlicePitch;
	};


	enum EStereoMethod
	{
		STM_None,
		STM_ColorAnaglyph_RedCyan,
		STM_ColorAnaglyph_YellowBlue,
		STM_ColorAnaglyph_GreenRed,
		STM_LCDShutter,
		STM_HorizontalInterlacing,
		STM_VerticalInterlacing,
		STM_Horizontal,
		STM_Vertical
	};
}

#endif			// _PixelFormat_HPP
