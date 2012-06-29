#include <D3D11/D3D11ObjFactory.h>
#include <D3D11/D3D11Texture.h>

namespace SE
{
			CTexturePtr MakeTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMaps, uint32 uArraySize,
			PixelFormat format, uint32 uSampleCount, uint32 SampleQuality, uint32 uAcces, PixelInitData const * cpInitData)
			{
				return MakeSharedPtr<D3D11Texture2D>(uWidth, uHeight, uNumMipMaps, uArraySize, format, uSampleCount, SampleQuality, uAcces, cpInitData);
			}
}