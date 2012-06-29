#include <SE3DEngine/Audio/CAudioDataSource.h>

namespace SE
{
	

	CAudioDataSource::~CAudioDataSource()
	{
	}

	EAudioFormat CAudioDataSource::GetFormat() const
	{
		return this->m_Format;
	}

	uint32 CAudioDataSource::GetFreq() const
	{
		return this->m_uFreq;
	}


}