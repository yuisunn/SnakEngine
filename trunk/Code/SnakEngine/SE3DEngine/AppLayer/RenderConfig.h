#ifndef _SRenderConfig_H_
#define _SRenderConfig_H_

#include <SE3DEngine/Comm/PixelFormat.h>
#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	struct SRenderConfig
	{
		SRenderConfig()
			:m_bFullScreen(false),m_ileft(0),m_iTop(0),m_ColorFmt(EF_ARGB8),
			m_DepthStencilFmt(EF_D16),m_uSampleCount(1),m_uSampleQuality(0),
			m_uMotionFrames(0),m_bHDR(false),m_bGamma(false),
			m_eStereoMethod(STM_None),m_fStereoSeparation(0)
		{}
		bool m_bFullScreen;
		int m_ileft;
		int m_iTop;
		int m_iWidth;
		int m_iHeight;
		PixelFormat m_ColorFmt;
		PixelFormat m_DepthStencilFmt;
		uint32 m_uSampleCount;
		uint32 m_uSampleQuality;
		uint32 m_uSyncInterval;
		uint32 m_uMotionFrames;
		bool m_bHDR;
		bool m_bGamma;

	    StereoMethod m_eStereoMethod;
		float m_fStereoSeparation;
	};
}

#endif