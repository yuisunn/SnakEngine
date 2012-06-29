//#include "DX9Render.h"
//
//
//namespace SE
//{
//	//構造方法
//	DX9Render::DX9Render()
//	{
//		m_pd3dDevice = GetDevice();
//
//	}
//	//vertex stream
//	HRESULT  DX9Render::FX_SetVStream(int nID, void *pB, uint32 nOffs, uint32 nStride, uint32 nFreq)
//	{
//		  D3DVertexBuffer *pVB = (D3DVertexBuffer *)pB;
//		  HRESULT h = S_OK;
//		  if (m_RP.m_VertexStreams[nID].pStream != pVB || m_RP.m_VertexStreams[nID].nOffset != nOffs)
//		  {
//			m_RP.m_VertexStreams[nID].pStream = pVB;
//			m_RP.m_VertexStreams[nID].nOffset = nOffs;
//#if defined (DIRECT3D9) || defined(OPENGL)
//			h = m_pd3dDevice->SetStreamSource(nID, pVB, nOffs, nStride);
//#elif defined (DIRECT3D10)
//			m_pd3dDeviceContext->IASetVertexBuffers(nID, 1, &pVB, &nStride, &nOffs);
//#endif
//		  }
//#if defined (DIRECT3D9) || defined(OPENGL)
//		  if (nFreq != -1 && m_RP.m_VertexStreams[nID].nFreq != nFreq)
//		  {
//			m_RP.m_VertexStreams[nID].nFreq = nFreq;
////#if !defined(XENON) && !defined(PS3)
//			//渲染多個 mesh stream 流得頻率分割
//			h = m_pd3dDevice->SetStreamSourceFreq(nID, nFreq);
//#endif
//		  }
//#endif
//		  assert(h == S_OK);
//		  return h;
//
//	}
//	//設indexstream
//	HRESULT DX9Render::FX_SetIStream(void* pB)
//	{
//			D3DIndexBuffer *pIB = (D3DIndexBuffer *)pB;
//			HRESULT h = S_OK;
//			if (m_RP.m_pIndexStream != pIB)
//			{
//			m_RP.m_pIndexStream = pIB;
//#if defined (DIRECT3D9) || defined(OPENGL)
//			h = m_pd3dDevice->SetIndices(pIB);
//#elif defined (DIRECT3D10)
//			m_pd3dDeviceContext->IASetIndexBuffer(pIB, DXGI_FORMAT_R16_UINT, 0);
//#endif
//			assert(h == S_OK);
//			}
//			return h;
//	}
//	//設貼圖
//	HRESULT DX9Render::FX_SetTextureAsVStream( int nID, CTexture *pVBTexture, uint32 nStride )
//	{
//		if(pVBTexture->GetDevTexture() == NULL)
//		{
//			assert(0);
//			return E_FAIL;
//		}
//
//#if defined (DIRECT3D9) || defined(OPENGL)
//	// Enable render to vertex buffer extension
//	HRESULT hr = m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, D3D9R2VBHelper::r2vbGlbEnable_Set(bSetVB));
//	assert(SUCCEEDED(hr));
//	if(bSetVB)
//	{
//		// Setup stream nID R2VB data裝載到 shader 對應 samples xx
//		hr = m_pd3dDevice->SetTexture(D3DVERTEXTEXTURESAMPLER0 + nID, pVBTexture->GetDevTexture()->Get2DTexture());
//		assert(SUCCEEDED(hr));
//		// Tell the driver that stream nID is to be fetched from the DMAP texture
//		hr = m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, D3D9R2VBHelper::r2vbVStrm2SmpMap_Set(nID, D3D9R2VBHelper::R2VB_VSMP_OVR_VTX0 + nID));
//		assert(SUCCEEDED(hr));
//	}
//	else
//	{
//		hr = m_pd3dDevice->SetTexture(D3DVERTEXTEXTURESAMPLER0 + nID, NULL);
//		assert(SUCCEEDED(hr));
//		// Stream 1 restored to regular vertex buffer mode
//		hr = m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, D3D9R2VBHelper::r2vbVStrm2SmpMap_Set(1, D3D9R2VBHelper::R2VB_VSMP_OVR_DIS));
//		assert(SUCCEEDED(hr));
//	}
//#elif defined (DIRECT3D10)
//	// the feature is not supported
//#endif
//	return S_OK;
//}
//
//	//設置頂點格式
//	HRESULT DX9Render::FX_SetVertexDeclaration(int StreamMask, EVertexFormat eVF)
//	{
//	    HRESULT hr;
//
//	    assert (eVF>=0 && eVF<eVF_Max);
//
//	    bool bMorph = (StreamMask&VSM_MORPHBUDDY)!=0;
//
//		SD3DVertexDeclarationCache *pDeclCache = &m_RP.m_D3DVertexDeclarationCache[(StreamMask&0xff)>>1][eVF][bMorph?1:0];
//
//		if (!pDeclCache->m_pDeclaration)
//	    {
//			SD3DVertexDeclaration Decl;
//
//			EF_VertexDeclaration(Decl,(StreamMask&0xff)>>1, eVF, bMorph);
//
//#if defined (DIRECT3D9) || defined(OPENGL)
//		if(FAILED(hr = m_pd3dDevice->CreateVertexDeclaration(&Decl.m_Declaration[0], &pDeclCache->m_pDeclaration)))
//		  return hr;
//#elif defined (DIRECT3D10)
//         ///帶填
//#endif
//	    }
//		  D3DVertexDeclaration *pD3DDecl = pDeclCache->m_pDeclaration;
//
//		  if (m_pLastVDeclaration != pD3DDecl)
//		  {
//#if defined (DIRECT3D9) || defined(OPENGL)
//			m_pLastVDeclaration = pD3DDecl;
//			return m_pd3dDevice->SetVertexDeclaration(pD3DDecl);
//#elif defined (DIRECT3D10) 
//			// Don't set input layout on fallback shader (crashes in DX11 NV driver)
//			//if (!CHWShader_D3D::m_pCurInstVS || CHWShader_D3D::m_pCurInstVS->m_bFallback)
//			//   return -1;
//			//m_pLastVDeclaration = pD3DDecl;
//			//m_pd3dDeviceContext->IASetInputLayout(pD3DDecl);
//#endif
//		  }
//#if defined (DIRECT3D10)
//			//// Don't render fallback in DX11
//			//if (!CHWShader_D3D::m_pCurInstVS || !CHWShader_D3D::m_pCurInstPS || CHWShader_D3D::m_pCurInstVS->m_bFallback || CHWShader_D3D::m_pCurInstPS->m_bFallback)
//			//return -1;
//#endif
//		  return S_OK;
//		}
//
//	void DX9Render::EF_VertexDeclaration( SD3DVertexDeclaration &out, const int nStreamMask, const int vertexformat, const bool bMorph )
//	{
//		if (!m_RP.m_D3DVertexDeclaration[vertexformat].m_Declaration.Num())
//		return;
//
//		uint32 j;
//
//#if defined (DIRECT3D9)
//	  for (j=0; j<m_RP.m_D3DVertexDeclaration[vertexformat].m_Declaration.Num()-1; j++)
//	  {
//		out.m_Declaration.push_back(m_RP.m_D3DVertexDeclaration[vertexformat].m_Declaration[j]);
//	  }
//	  for (j=1; j<VSF_NUM; j++)
//	  {
//		if (!(nStreamMask & (1<<(j-1))))
//		  continue;
//		int n = 0;
//		//0xff = 255
//		while (m_RP.m_D3DVertexElement[j].m_pElements[n].Stream != 0xff)
//		{
//		  out.m_Declaration.push_back(m_RP.m_D3DVertexElement[j].m_pElements[n]); 
//		  n++;
//		}
//	  }
//
//		if(bMorph)
//		{
//			uint32 dwNumWithoutMorph = out.m_Declaration.Num();
//
//			for (j=0; j<dwNumWithoutMorph; j++)
//			{
//				D3DVERTEXELEMENT9 El = out.m_Declaration[j];
//				El.Stream += VSF_MORPHBUDDY;
//				El.UsageIndex += 8;
//				out.m_Declaration.push_back(El);
//			}
//		
//			D3DVERTEXELEMENT9 El = {VSF_MORPHBUDDY_WEIGHTS, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1};	// BlendWeight
//			out.m_Declaration.push_back(El);
//		}
//
//		D3DVERTEXELEMENT9 elemEnd = D3DDECL_END();         // terminate
//	  out.m_Declaration.push_back(elemEnd);
//
//#elif defined (DIRECT3D10)
//	  for (j=0; j<m_RP.m_D3DVertexDeclaration[vertexformat].m_Declaration.Num(); j++)
//	  {
//		out.m_Declaration.push_back(m_RP.m_D3DVertexDeclaration[vertexformat].m_Declaration[j]);
//	  }
//	  for (j=1; j<VSF_NUM; j++)
//	  {
//		if (!(nStreamMask & (1<<(j-1))))
//		  continue;
//		int n;
//			for (n=0; n<m_RP.m_D3DVertexElement[j].m_nNumElements; n++)
//		{
//		  out.m_Declaration.push_back(m_RP.m_D3DVertexElement[j].m_pElements[n]); 
//		}
//	  }
//
//		if(bMorph)
//		{
//			uint32 dwNumWithoutMorph = out.m_Declaration.Num();
//
//			for (j=0; j<dwNumWithoutMorph; j++)
//			{
//				D3D11_INPUT_ELEMENT_DESC El = out.m_Declaration[j];
//				El.InputSlot += VSF_MORPHBUDDY;
//				El.SemanticIndex += 8;
//				out.m_Declaration.AddElem(El);
//			}
//			D3D11_INPUT_ELEMENT_DESC El = {"BLENDWEIGHT", 1, DXGI_FORMAT_R32G32_FLOAT, VSF_MORPHBUDDY_WEIGHTS, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}; // BlendWeight
//			out.m_Declaration.AddElem(El);
//		}
//#endif
//	}
//
//	//設置渲染狀態
//	void CD3D9Renderer::EF_SetState(int st, int AlphaRef, int RestoreState)
//	{
//	  int Changed;
//  
//	  Changed = st ^ m_RP.m_CurState;
//
//	  Changed |= RestoreState;
//
//	  if (!Changed && (AlphaRef==-1 || AlphaRef==m_RP.m_CurAlphaRef))  
//
//		return;
//
//#if defined (DIRECT3D9) || defined(OPENGL)
//	  int src, dst;
//	  LPDIRECT3DDEVICE9 dv = gcpRendD3D->GetD3DDevice();
//	  m_RP.m_PS[m_RP.m_nProcessThreadID].m_NumStateChanges++;
//	  if (Changed & GS_DEPTHFUNC_MASK)
//	  {
//		switch (st & GS_DEPTHFUNC_MASK)
//		{
//		case GS_DEPTHFUNC_EQUAL:
//		  m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
//		  break;
//		case GS_DEPTHFUNC_LEQUAL:
//		  m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
//		  break;
//		case GS_DEPTHFUNC_GREAT:
//		  m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
//		  break;
//		case GS_DEPTHFUNC_LESS:
//		  m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
//		  break;
//		case GS_DEPTHFUNC_NOTEQUAL:
//		  m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NOTEQUAL);
//		  break;
//		case GS_DEPTHFUNC_GEQUAL:
//		  m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
//		  break;
//		}
//	  }
//	  m_RP.m_CurState = st;
//	}
//
//
//}