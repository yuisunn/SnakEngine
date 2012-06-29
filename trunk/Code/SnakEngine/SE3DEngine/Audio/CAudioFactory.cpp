#include <SE3DEngine/Audio/CAudioFactory.h>

namespace SE
{

	CAudio& CAudioFactory::InstAudio()
	{
		if (!m_pAudio)
		{
			m_pAudio = this->MakeAudio();
		}

		return *m_pAudio;
	}
}