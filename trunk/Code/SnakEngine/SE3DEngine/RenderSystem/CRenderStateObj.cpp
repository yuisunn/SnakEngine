#include <SE3DEngine/RenderSystem/CRenderStateObj.h>
#include <limits>
#include <boost/assert.hpp>

namespace SE
{
	SamplerStateDesc::SamplerStateDesc()
	: m_BorderClr(0.0f),
		m_eAddrModeU(TAM_Wrap), m_eAddrModeV(TAM_Wrap), m_eAddrModeW(TAM_Wrap),
		m_eFilter(TFO_Min_Mag_Mip_Point),
		m_uMaxAnisotropy(16),
		m_fMinLod(0), m_fMaxLod((std::numeric_limits<float>::max)()),
		m_fMipMapLodBias(0),
		m_eCFunc(CF_AlwaysFail)
	{
	}
}