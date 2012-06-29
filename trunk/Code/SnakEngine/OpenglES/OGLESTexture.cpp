#include <OpenglES/OGLESTexture.h>

#include <boost/typeof/typeof.hpp>

namespace SE
{
	OGLESTexture::OGLESTexture(ETextureType eType,uint32 uArraySize, uint32 uSample, uint32 uQuality, uint32 uAccess)
		:CTexture(eType,uSample,uQuality,uAccess)
	{}
	OGLESTexture::~OGLESTexture()
	{
		//glDeleteTexture(1,&m_iTex);
	}
	void OGLESTexture::BindTexture()
	{
		glBindTexture(m_eGLTexType,m_iGLTex);
	}

	void OGLESTexture::BuildMipSubLevels()
	{
		this->BindTexture();
		glGenerateMipmap(m_eGLTexType);
	}

	void OGLESTexture::TexParameteri(GLenum eName, GLint iParam)
	{
		BOOST_AUTO(iter, m_TexParamIs.find(eName));
		if ((iter == m_TexParamIs.end()) || (iter->second != iParam))
		{
			glBindTexture(m_eGLTexType, m_iGLTex);
			glTexParameteri(m_eGLTexType, eName, iParam);

			m_TexParamIs[eName] = iParam;
		}
	}

	void OGLESTexture::TexParameterf(GLenum eName, GLfloat fParam)
	{
		BOOST_AUTO(iter, m_TexParamFs.find(eName));
		if ((iter == m_TexParamFs.end()) || (iter->second != fParam))
		{
			glBindTexture(m_eGLTexType, m_iGLTex);
			glTexParameterf(m_eGLTexType, eName, fParam);

			m_TexParamFs[eName] = fParam;
		}
	}
}
