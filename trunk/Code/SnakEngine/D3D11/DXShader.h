#ifndef _DXShader_H_
#define _DXShader_H_

#include <SE3DEngine/RenderSystem/IShader.h>
#include <SE3DEngine/RenderSystem/RenderPipeline.h>
#include <d3dx9effect.h>
#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	//class DX9Shader : public IShader
	//{
	//public:
	//	DX9Shader(void);
	//	~DX9Shader(void);

	//	bool FXSetTechnique(const String& Name);
	//    bool FXBegin(uint32 *nPassCount, uint32 nFlags);
	//	bool FXBeginPass(uint32 nPass);
	//	bool FXCommit(const uint32 nFlags);
	//	bool FXEndPass();
	//	bool FXEnd();
	//    virtual ShSetTechnique(char *cTachName);
	//    virtual ShBeginPass(uint nPass);
	//    virtual ShEndPass();
	//	virtual const char *GetName() const;
	//	virtual int GetFlags();
	//	virtual bool ShLoadEffectFile(int nFlags, const char *sShaderName);
	//	virtual std::vector<SShaderParam>& ShGetParams();
	//	virtual int GetTexId ();
	//	virtual ITexture* GetBaseTexture(int *nPass, int *nTU);
	//	virtual unsigned int GetUsedTextureTypes(void);
	//	virtual int Size(int Flags);
	//	virtual int GetTechniqueID(int nTechnique, int nRegisteredTechnique);
	//	virtual EVertexFormat GetVertexFormat(void);
	//	virtual EShaderType GetShaderType();
	//protected:
	//	std::vector<SShaderTechniquePtr> m_pTechniqueList;
	//	std::vector<SShaderParamPtr> m_pParamList;
	//	RenderPipeline m_RP;
	//	EVertexFormat m_eVertexFormat;
	//	String m_sName;
	//	EShaderType m_eShaderType;
	//	LPD3DXEFFECT m_pEffect;

	//};


}
#endif
