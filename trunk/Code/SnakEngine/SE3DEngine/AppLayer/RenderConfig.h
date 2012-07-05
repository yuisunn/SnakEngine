#ifndef _SRenderConfig_H_
#define _SRenderConfig_H_

#include <SE3DEngine/Comm/PixelFormat.h>
#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	struct SRenderConfig
	{
		SRenderConfig()
			:bFullScreen(false),iLeft(0),iTop(0),ColorFmt(EF_ARGB8),
			DepthStencilFmt(EF_D16),uSampleCount(1),uSampleQuality(0),
			uMotionFrames(0),bHDR(false),bGamma(false),
			eStereoMethod(STM_None),fStereoSeparation(0)
		{}
		bool bFullScreen;
		int iLeft;
		int iTop;
		int iWidth;
		int iHeight;
		PixelFormat ColorFmt;
		PixelFormat DepthStencilFmt;
		uint32 uSampleCount;
		uint32 uSampleQuality;
		uint32 uSyncInterval;
		uint32 uMotionFrames;
		bool bHDR;
		bool bGamma;
		bool bPPAA;
		bool bColorGrading;
	    EStereoMethod eStereoMethod;
		float fStereoSeparation;
	};
}

#endif