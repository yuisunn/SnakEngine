#include <OpenglES/OGLESObjFactory.h>

#include <SE3DEngine/Comm/Util.h>
#include <OpenglES/OGLESTexture.h>


namespace SE
{
			CTexturePtr OGLESObjFactory::MakeTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMaps, uint32 uArraySize,
			PixelFormat format, uint32 uSampleCount, uint32 SampleQuality, uint32 uAcces, PixelInitData const * cpInitData)
			{
				return MakeSharedPtr<OGLESTexture2D>(uWidth, uHeight, uNumMipMaps, uArraySize, format, uSampleCount, SampleQuality, uAcces, cpInitData);
			}
}