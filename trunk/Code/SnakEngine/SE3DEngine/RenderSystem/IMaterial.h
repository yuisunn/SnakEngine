#ifndef _Material_H_
#define _Material_H_

#include <SE3DEngine/RenderSystem/IShader.h>
#include <SE3DEngine/Comm/Def.h>

namespace SE
{

	struct IMaterial
	{
		virtual String GetName();
		virtual void SetShader(IShader* pShader);
		virtual void SetCamera(Camera* pCamera);
		virtual void SetShaderParam(EShaderParamType eType, UShaderParamVal uVal);
		virtual void SetIsTransparent(bool bTransparent);
		virtual void SetAlpha(float nAlpha);

		String m_sName;
		IShader* m_pShader;
		Camera* m_pCamera;
		bool m_bTransparent; //Í¸Ã÷
		int8 m_nPriority;
		std::vector<SShaderParamPtr> m_pShaderParams;
		float m_fAlpha;
	};

}
#endif