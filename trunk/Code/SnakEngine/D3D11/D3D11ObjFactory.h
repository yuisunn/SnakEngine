#ifndef _D3D11ObjFactory_H_
#define _D3D11ObjFactory_H_

#include <SE3DEngine/RenderSystem/CAPIObjFactory.h>
#include <D3D11/D3D11Render.h>
#include <SE3DEngine/Comm/Util.h>

namespace SE
{
	class _DLL_API D3D11ObjFactory : public CAPIObjFactory
	{
	public:
		
		CTexturePtr MakeTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMaps, uint32 uArraySize,
			PixelFormat format, uint32 uSampleCount, uint32 SampleQuality, uint32 uAcces, PixelInitData const * cpInitData);
	private:
		CRenderPtr MakeRender()
		{
			return MakeSharedPtr<D3D11Render>();
		}
	};

}

#endif