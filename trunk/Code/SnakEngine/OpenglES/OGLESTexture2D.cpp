#include <OpenglES/OGLESTexture.h>

#include <boost/assert.hpp>
#include <SE3DEngine/Comm/Util.h>




#pragma warning(disable: 4018)



namespace SE
{
    OGLESTexture2D::OGLESTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMip, uint32 uArraySize, PixelFormat Format,
	    uint32 uSampleCount, uint32 uSampleQuality, uint32 uAccess, PixelInitData const * cpInitData)
	    :OGLESTexture(TT_2D,uArraySize,uSampleCount,uSampleQuality,uAccess)
    {}
    OGLESTexture2D::~OGLESTexture2D()
    {}

    uint32 OGLESTexture2D::GetWidth(uint32 uLevel) const
    {
	    BOOST_ASSERT(uLevel < m_iNumMip);
	    return m_uWidthes[uLevel];
    }
    uint32 OGLESTexture2D::GetHeight(uint32 uLevel) const
    {
	    BOOST_ASSERT(uLevel < m_iNumMip);
	    return m_uHeights[uLevel];
    }
	//复制2d贴图
	void OGLESTexture2D::CopyToTexture(CTexture& pTarget)
    {
	    BOOST_ASSERT(m_eTexType == pTarget.GetType());
	    if((m_Format == pTarget.GetFormat()) && (m_uWidthes[0] == pTarget.GetWidth(0)) && (m_uHeights[0] == pTarget.GetHeight(0)) )
	    {
		    uint32 texel_size = NumFormatBytes(m_Format);
		    GLint gl_internalFormat;
		    GLenum gl_format;
		    GLenum gl_type;
		    OGLESMapping::MappingFormat(gl_internalFormat, gl_format, gl_type, m_Format);//一些参数映射
		    OGLESTexture2D& gl_target = *checked_cast<OGLESTexture2D*>(&pTarget);
		    for(uint32 uLevel=0; uLevel<m_iNumMip; ++uLevel)
		    {
			    glBindTexture(m_eTexType,gl_target.GetGLTex());
			    if(IsCompressedFormat(pTarget.GetFormat()))
			    {
				    //1像素大小
				    int block_size;
				    if ((EF_BC1 == m_Format) || (EF_SIGNED_BC1 == m_Format) || (EF_BC1_SRGB == m_Format)
				        || (EF_BC4 == m_Format) || (EF_SIGNED_BC4 == m_Format) || (EF_BC4_SRGB == m_Format))
			        {
				        block_size = 8;
			        }
			        else
			        {
				        block_size = 16;
			        }
				    //图片数据部分大小
				    GLsizei const image_size = ((pTarget.GetWidth(uLevel) + 3) / 4) * ((pTarget.GetHeight(uLevel) + 3) / 4) * block_size;

			        memcpy(&gl_target. m_uTexDatas[uLevel][0], &m_uTexDatas[uLevel][0], image_size);
				    glCompressedTexSubImage2D(m_eTexType, uLevel, 0, 0,this->GetWidth(uLevel), this->GetHeight(uLevel), gl_format, image_size, &m_uTexDatas[uLevel][0]);
			    }
		    }

	    }
	    else
	    {
		    for (uint32 uArrayIndex = 0; uArrayIndex < m_iArraySize; ++ uArrayIndex)
	        {
		         for (uint32 uLevel = 0; uLevel < m_iNumMip; ++ uLevel)
		         {
			        this->CopyToSubTexture2D(pTarget,
					    uArrayIndex, uLevel, 0, 0, pTarget.GetWidth(uLevel), pTarget.GetHeight(uLevel),
				    uArrayIndex, uLevel, 0, 0, this->GetWidth(uLevel), this->GetWidth(uLevel));
		        }
	        }
	    }


    }
	//局部修改对2d贴图
    void OGLESTexture2D::CopyToSubTexture2D(CTexture& pTarget,
	    uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
	    uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height)
	{
		BOOST_ASSERT(this->m_eTexType == pTarget.GetType());
		BOOST_ASSERT(m_Format == pTarget.GetFormat());

		GLint gl_internalFormat;
		GLenum gl_format;
		GLenum gl_type;
		OGLESMapping::MappingFormat(gl_internalFormat, gl_format, gl_type, m_Format);

		GLint gl_target_internal_format;
		GLenum gl_target_format;
		GLenum gl_target_type;
		OGLESMapping::MappingFormat(gl_target_internal_format, gl_target_format, gl_target_type, pTarget.GetFormat());

		if (IsCompressedFormat(m_Format))
		{
			BOOST_ASSERT((uSrc_Width == uTar_Width) && (uSrc_Width == uTar_Height));
			BOOST_ASSERT((0 == (uSrc_XOffset & 0x3)) && (0 == (uSrc_YOffset & 0x3)));
			BOOST_ASSERT((0 == (uTar_XOffset & 0x3)) && (0 == (uTar_YOffset & 0x3)));
			BOOST_ASSERT((0 == (uSrc_Width & 0x3)) && (0 == (uSrc_Width & 0x3)));
			BOOST_ASSERT((0 == (uTar_Width & 0x3)) && (0 == (uTar_Height & 0x3)));

			CTexture::Mapper mapper_src(*this, uSrc_ArrayIndex, uSrc_Level, TAT_Read_Only, 0, 0, this->GetWidth(uSrc_Level), this->GetHeight(uSrc_Level));
			CTexture::Mapper mapper_dst(pTarget, uTar_ArrayIndex, uTar_Level, TAT_Write_Only, 0, 0, pTarget.GetWidth(uTar_Level), pTarget.GetHeight(uTar_Level));

			int block_size;
			if ((EF_BC1 == m_Format) || (EF_SIGNED_BC1 == m_Format) || (EF_BC1_SRGB == m_Format)
				|| (EF_BC4 == m_Format) || (EF_SIGNED_BC4 == m_Format) || (EF_BC4_SRGB == m_Format))
			{
				block_size = 8;
			}
			else
			{
				block_size = 16;
			}

			uint8 const * s = mapper_src.Pointer<uint8>() + (uSrc_YOffset / 4) * mapper_src.RowPitch() + (uSrc_XOffset / 4 * block_size);
			uint8* d = mapper_dst.Pointer<uint8>() + (uTar_YOffset / 4) * mapper_dst.RowPitch() + (uTar_XOffset / 4 * block_size);
			for (uint32 y = 0; y < uSrc_Width; y += 4)
			{
				memcpy(d, s, uSrc_Width / 4 * block_size);

				s += mapper_src.RowPitch();
				d += mapper_dst.RowPitch();
			}
		}
		else
		{
			size_t const src_format_size = NumFormatBytes(m_Format);
			size_t const dst_format_size = NumFormatBytes(pTarget.GetFormat());

			if ((uSrc_Width != uTar_Width) || (uSrc_Width != uTar_Height))
			{
#ifndef SE_ANDROID
				std::vector<uint8> data_in(uSrc_Width * uSrc_Width * src_format_size);
				std::vector<uint8> data_out(uTar_Width * uTar_Height * dst_format_size);

				{
					CTexture::Mapper mapper(*this, uSrc_ArrayIndex, uSrc_Level, TAT_Read_Only, uSrc_XOffset, uSrc_YOffset, uSrc_Width, uSrc_Width);
					uint8 const * s = mapper.Pointer<uint8>();
					uint8* d = &data_in[0];
					for (uint32 y = 0; y < uSrc_Width; ++ y)
					{
						memcpy(d, s, uSrc_Width * src_format_size);

						s += mapper.RowPitch();
						d += uSrc_Width * src_format_size;
					}
				}

			//	gluScaleImage(gl_format, uSrc_Width, uSrc_Width, gl_type, &data_in[0],
				//	uTar_Width, uTar_Height, gl_target_type, &data_out[0]);

				{
					CTexture::Mapper mapper(pTarget, uTar_ArrayIndex, uTar_Level, TAT_Write_Only, uTar_XOffset, uTar_YOffset, uTar_Width, uTar_Height);
					uint8 const * s = &data_out[0];
					uint8* d = mapper.Pointer<uint8>();
					for (uint32 y = 0; y < uTar_Height; ++ y)
					{
						memcpy(d, s, uTar_Width * dst_format_size);

						s += uTar_Width * src_format_size;
						d += mapper.RowPitch();
					}
				}
#else
				BOOST_ASSERT(false);
#endif
			}
			else
			{
				CTexture::Mapper mapper_src(*this, uSrc_ArrayIndex, uSrc_Level, TAT_Read_Only, uSrc_XOffset, uSrc_YOffset, uSrc_Width, uSrc_Width);
				CTexture::Mapper mapper_dst(pTarget, uTar_ArrayIndex, uTar_Level, TAT_Write_Only, uTar_XOffset, uTar_YOffset, uTar_Width, uTar_Height);
				uint8 const * s = mapper_src.Pointer<uint8>();
				uint8* d = mapper_dst.Pointer<uint8>();
				for (uint32_t y = 0; y < uSrc_Width; ++ y)
				{
					memcpy(d, s, uSrc_Width * src_format_size);

					s += mapper_src.RowPitch();
					d += mapper_dst.RowPitch();
				}
			}
		}

	}
    void OGLESTexture2D::Map2D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
	    uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
	    void*& prData, uint32& urRowPitch)
	{
		BOOST_ASSERT(0 == uArrayIndex);
		UNREF_PARAM(uArrayIndex);

		m_eAccessType = eAcessType;

		uint32_t const texel_size = NumFormatBytes(m_Format);
		int block_size;
		if (IsCompressedFormat(m_Format))
		{
			if ((EF_BC1 == m_Format) || (EF_SIGNED_BC1 == m_Format) || (EF_BC1_SRGB == m_Format)
				|| (EF_BC4 == m_Format) || (EF_SIGNED_BC4 == m_Format) || (EF_BC4_SRGB == m_Format))
			{
				block_size = 8;
			}
			else
			{
				block_size = 16;
			}
		}
		else
		{
			block_size = 0;
		}

		urRowPitch = m_uWidthes[uLevel] * texel_size;

		uint8* p = &m_uTexDatas[uLevel][0];
		if (IsCompressedFormat(m_Format))
		{
			prData = p + (uYOffset / 4) * urRowPitch + (uXOffset / 4 * block_size);
		}
		else
		{
			prData = p + (uYOffset * m_uWidthes[uLevel] + uXOffset) * texel_size;
		}
	}
    void OGLESTexture2D::UnMap2D(uint32 uArrayIndex, uint32 uLevel)
	{
		BOOST_ASSERT(0 == uArrayIndex);
		UNREF_PARAM(uArrayIndex);

		switch (m_eAccessType)
		{
		case TAT_Read_Only:
			break;

		case TAT_Write_Only:
		case TAT_Read_Write:
			{
				GLint gl_internalFormat;
				GLenum gl_format;
				GLenum gl_type;
				OGLESMapping::MappingFormat(gl_internalFormat, gl_format, gl_type, m_Format);

				glBindTexture(m_eGLTexType, m_iGLTex);

				if (IsCompressedFormat(m_Format))
				{
					int block_size;
					if ((EF_BC1 == m_Format) || (EF_SIGNED_BC1 == m_Format) || (EF_BC1_SRGB == m_Format)
						|| (EF_BC4 == m_Format) || (EF_SIGNED_BC4 == m_Format) || (EF_BC4_SRGB == m_Format))
					{
						block_size = 8;
					}
					else
					{
						block_size = 16;
					}

					GLsizei const image_size = ((m_uWidthes[uLevel] + 3) / 4) * ((m_uHeights[uLevel] + 3) / 4) * block_size;

					glCompressedTexSubImage2D(m_eGLTexType, uLevel, 0, 0,
						m_uWidthes[uLevel], m_uHeights[uLevel], gl_format, image_size, &m_uTexDatas[uLevel][0]);
				}
				else
				{
					glTexSubImage2D(m_eGLTexType, uLevel, 0, 0, m_uWidthes[uLevel], m_uHeights[uLevel],
							gl_format, gl_type, &m_uTexDatas[uLevel][0]);
				}
			}
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

	}





}