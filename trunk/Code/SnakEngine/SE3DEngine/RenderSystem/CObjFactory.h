#ifndef _CObjFactory_H_
#define _CObjFactory_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/PixelFormat.h>
#include <SE3DEngine/RenderSystem/CRenderStateObj.h>
#include <map>

namespace SE
{
	class _DLL_API CObjFactory
	{
	public:
		CObjFactory();
		virtual ~CObjFactory();

		static CObjFactoryPtr NullObject();
		CRender& InstRender()
		{
			if(!m_pRen)
			{
				m_pRen = MakeRender();
			}
			return *m_pRen;
		}
		
		virtual CTexturePtr MakeTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMaps, uint32 uArraySize,
			PixelFormat format, uint32 uSampleCount, uint32 SampleQuality, uint32 uAccessHint, PixelInitData const * cpInitData) = 0;
	    //创建 shader virtual
		//创建 render state
	private:
		virtual CRenderPtr MakeRender() = 0;
		CRenderPtr m_pRen;
		std::map<RasterizerStateDesc, CRasterizerStateObjPtr> m_RasStatePool;
		std::map<DepthStencilStateDesc, CDepthStencilStateObjPtr> m_DepthStenStatePool;
		std::map<BlendStateDesc, CBlendStateObjPtr> m_BlendStatePool;
		std::map<SamplerStateDesc, CSamplerStateObjPtr> m_SamplerStatePool;

	};
}

#endif

