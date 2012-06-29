#ifndef _DX9Render_H_
#define _DX9Render_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/RenderSystem/IRender.h>
#include <SE3DEngine/Image/ITexture.h>
#include <vector>

namespace SE
{
	class DX9Render : public IRender
	{
	public:
	   DX9Render();
	   ~DX9Render();
 
	//   HRESULT  FX_SetVStream(int nID, void *pB, uint32 nOffs, uint32 nStride, uint32 nFreq=1);
	//   HRESULT  FX_SetIStream(void* pB);
	//   HRESULT  FX_SetTextureAsVStream( int nID, ITexture *pVBTexture, uint32 nStride );
	//   void EF_VertexDeclaration( SD3DVertexDeclaration &out, const int nStreamMask, const int vertexformat, const bool bMorph );
	//   void FX_Commit(bool bAllowDIP);
	//   void EF_SetState(int st, int AlphaRef, int RestoreState);
	//   void FX_CommitStates();
	//private:
	//	LPDIRECT3DDEVICE9 m_pd3dDevice;
	//	RenderPipeline m_RP;

	};

}

#endif