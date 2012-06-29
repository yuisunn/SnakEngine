#include <SE3DEngine/Image/CTexture.h>
#include <SE3DEngine/Comm/Util.h>

namespace SE
{
	CTexture::CTexture(ETextureType eType, uint32 uSample, uint32 uQuality, uint32 uAccess)
		: m_Format(eType),m_uSample(uSample),m_uSampleQuality(uQuality),m_uAccess(uAccess)
		
	{
	}
	CTexture::~CTexture()
	{
	}


	uint32 CTexture::GetNumMip() const
	{
		return m_iNumMip;
	}

	uint32 CTexture::GetArraySize() const
	{
		return m_iArraySize;
	}

	PixelFormat CTexture::GetFormat() const
	{
		return m_Format;
	}

	ETextureType CTexture::GetType() const
	{
		return m_eTexType;
	}

	uint32 CTexture::GetSampleCount() const
	{
		return m_uSample;
	}

	uint32 CTexture::GetSampleQuality() const
	{
		return m_uSampleQuality;
	}

	uint32 CTexture::GetAccess() const
	{
		return m_uAccess;
	}


}