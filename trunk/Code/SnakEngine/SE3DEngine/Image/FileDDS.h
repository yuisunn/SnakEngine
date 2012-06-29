#ifndef _FileDDS_H_
#define _FileDDS_H_
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Image/CTexture.h>

namespace SE
{
		//dds数据定义
	enum
	{
		// The surface has alpha channel information in the pixel format.
		DDSPF_ALPHAPIXELS = 0x00000001,

		// The pixel format contains alpha only information
		DDSPF_ALPHA = 0x00000002,

		// The FourCC code is valid.
		DDSPF_FOURCC = 0x00000004,

		// The RGB data in the pixel format structure is valid.
		DDSPF_RGB = 0x00000040,

		// Luminance data in the pixel format is valid.
		// Use this flag for luminance-only or luminance+alpha surfaces,
		// the bit depth is then ddpf.dwLuminanceBitCount.
		DDSPF_LUMINANCE = 0x00020000,

		// Bump map dUdV data in the pixel format is valid.
		DDSPF_BUMPDUDV = 0x00080000
	};
	enum
	{
		// This flag is used at CreateSurface time to indicate that this set of
		// surfaces is a cubic environment map
		DDSCAPS2_CUBEMAP	= 0x00000200,

		// These flags preform two functions:
		// - At CreateSurface time, they define which of the six cube faces are
		//   required by the application.
		// - After creation, each face in the cubemap will have exactly one of these
		//   bits set.
		DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400,
		DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800,
		DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000,
		DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000,
		DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000,
		DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000,

		// Indicates that the surface is a volume.
		// Can be combined with DDSCAPS_MIPMAP to indicate a multi-level volume
		DDSCAPS2_VOLUME		= 0x00200000,
	};
	enum
	{
		// Indicates a complex surface structure is being described.  A
		// complex surface structure results in the creation of more than
		// one surface.  The additional surfaces are attached to the root
		// surface.  The complex structure can only be destroyed by
		// destroying the root.
		DDSCAPS_COMPLEX		= 0x00000008,

		// Indicates that this surface can be used as a 3D texture.  It does not
		// indicate whether or not the surface is being used for that purpose.
		DDSCAPS_TEXTURE		= 0x00001000,

		// Indicates surface is one level of a mip-map. This surface will
		// be attached to other DDSCAPS_MIPMAP surfaces to form the mip-map.
		// This can be done explicitly, by creating a number of surfaces and
		// attaching them with AddAttachedSurface or by implicitly by CreateSurface.
		// If this bit is set then DDSCAPS_TEXTURE must also be set.
		DDSCAPS_MIPMAP		= 0x00400000,
	};
	enum
	{
		DDSD_CAPS			= 0x00000001,	// default, dds_caps field is valid.
		DDSD_HEIGHT			= 0x00000002,	// height field is valid.
		DDSD_WIDTH			= 0x00000004,	// width field is valid.
		DDSD_PITCH			= 0x00000008,	// pitch is valid.
		DDSD_PIXELFORMAT	= 0x00001000,	// pixel_format is valid.
		DDSD_MIPMAPCOUNT	= 0x00020000,	// mip_map_count is valid.
		DDSD_LINEARSIZE		= 0x00080000,	// linear_size is valid
		DDSD_DEPTH			= 0x00800000,	// depth is valid
	};
	
	//dds 文件头定义
	struct DDS_CAPS2
	{
		uint32	uCaps1;			// capabilities of surface wanted
		uint32	uCaps2;
		uint32	ugReserved[2];
	};
	struct DDS_PIXELFORMAT
	{
		uint32	uSize;				// size of structure
		uint32	uFlags;				// pixel format flags
		uint32	uFourCC;			// (FOURCC code)
		uint32	uRGB_bit_count;		// how many bits per pixel
		uint32	uR_bit_mask;			// mask for red bit
		uint32	uG_bit_mask;			// mask for green bits
		uint32	uB_bit_mask;			// mask for blue bits
		uint32	uRGB_alpha_bit_mask;	// mask for alpha channels
	};
	
	enum D3D10_RESOURCE_DIMENSION
	{
		D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
		D3D10_RESOURCE_DIMENSION_BUFFER = 1,
		D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
		D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
		D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4,
	};

	enum D3D10_RESOURCE_MISC_FLAG
    {
		D3D10_RESOURCE_MISC_GENERATE_MIPS = 0x1L,
		D3D10_RESOURCE_MISC_SHARED = 0x2L,
		D3D10_RESOURCE_MISC_TEXTURECUBE	= 0x4L,
		D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x10L,
		D3D10_RESOURCE_MISC_GDI_COMPATIBLE = 0x20L
    };

	struct DDS_HEADER_DXT10
	{
		uint32 uDXgiFormat;
		D3D10_RESOURCE_DIMENSION eResourceDim;
		uint32 uMiscFlag;
		uint32 uArraySize;
		uint32 uReserved;
	};
	struct DDS_HEADER
	{
		uint32	uSize;					// size of the DDSURFACEDESC structure
		uint32	uFlags;					// 那些数据有效
		uint32	uHeight;					// height of surface to be created
		uint32	uWidth;					// width of input surface
		union
		{
			int32		iPitch;				// distance to start of next line (return value only)
			uint32	uLinearSize;		// Formless late-allocated optimized surface size
		};
		uint32		uDepth;				// the depth if this is a volume texture
		uint32		uMip_map_count;		// number of mip-map levels requestde
		uint32		ugReserved1[11];		// reserved
		DDS_PIXELFORMAT	pixelFormat;		// pixel format description of the surface
		DDS_CAPS2		ddsCaps;			// direct draw surface capabilities
		uint32		uReserved2;
	};


   
    void LoadDDS(TexDesc& pDesc);
}
#endif // _GUTDDS_