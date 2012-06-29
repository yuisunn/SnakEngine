//负责es texture buffer depth stateobj等 的创建
#ifndef _OGLESRenderFactory_H_
#define _OGLESRenderFactory_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/Util.h>
#include <SE3DEngine/RenderSystem/CAPIObjFactory.h>
#include <OpenglES/OGLESRender.h>

namespace SE
{
	class OGLESObjFactory : public CAPIObjFactory
	{
	public:
		OGLESObjFactory();
		~OGLESObjFactory();


		CTexturePtr MakeTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMaps, uint32 uArraySize,
			PixelFormat format, uint32 uSampleCount, uint32 SampleQuality, uint32 uAccessHint, PixelInitData const * cpInitData);
	private:
		CRenderPtr MakeRender()
		{
			return MakeSharedPtr<OGLESRender>();
		}

	};

}

#endif

