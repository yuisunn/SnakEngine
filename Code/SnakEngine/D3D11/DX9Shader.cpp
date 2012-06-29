//#include "DX9Shader.h"
//
//namespace SE
//{
//		bool FXSetTechnique(const String& Name);
//		//設置要渲染的pass
//	    bool DX9Shader::FXBegin(uint32 *nPassCount, uint32 nFlags)
//		{
//			if (!rd->m_RP.m_pShader || !rd->m_RP.m_pCurTechnique)
//                return false;
//			*uiPassCount = rd->m_RP.m_pCurTechnique->m_Passes.Num();
//			rd->m_RP.m_nFlagsShaderBegin = nFlags;
//			rd->m_RP.m_pCurPass = &rd->m_RP.m_pCurTechnique->m_Passes[0];
//			return true;
//		}
//		// 設置要顯然pass 的vs ps
//		bool DX9Shader::FXBeginPass(uint32 nPass)
//		{
//			  if (!m_RP.m_pShader || !m_RP.m_pCurTechnique || !m_RP.m_pCurTechnique->m_Passes.Num())
//				return false;
//			  m_RP.m_pCurPass = &m_RP.m_pCurTechnique->m_Passes[nPass];
//			  SShaderPass *pPass = m_RP.m_pCurPass;
//			  assert (pPass->m_VShader && pPass->m_PShader);
//			  if (!pPass->m_VShader || !pPass->m_PShader)
//				return false;
//			  SShaderHandle* curVS = (SShaderHandle*)pPass->m_VShader;
//			  SShaderHandle* curPS = (SShaderHandle*)pPass->m_PShader;
//#if defined (DIRECT3D10)
//			  SShaderHandle* curGS = (SShaderHandle*)pPass->m_GShader;
//#endif
//#if defined (DIRECT3D10) && !defined(PS3)
//			  // Set Geometry shader
//#endif
//
//			  if (!(m_RP.m_nFlagsShaderBegin & FEF_DONTSETSTATES))
//			  {
//				EF_SetState(pPass->m_RenderState);
//				if (pPass->m_eCull != -1)
//				  D3DSetCull((ECull)pPass->m_eCull);
//			  }
//			  //setparam
//			  curVS->SetParametersPI(NULL, rd->m_RP.m_pShader);
//			  curPS->SetParametersPI(NULL, NULL);
//
//			  return true;
//		}
//		bool FXCommit(const uint32 nFlags)
//		{
//			//set param common
//			//set rendertarget
//		}
//		//吧當前 pass 置空
//		bool FXEndPass()
//		{
//			if (!m_RP.m_pShader || !m_RP.m_pCurTechnique || !m_RP.m_pCurTechnique->m_Passes.Num())
//			return false;
//			rd->m_RP.m_pCurPass = NULL;
//			return true;
//		}
//		//
//		bool FXEnd()
//		{
//			if (!m_RP.m_pShader || !m_RP.m_pCurTechnique)
//				return false;
//			return true;
//		}
//
//}