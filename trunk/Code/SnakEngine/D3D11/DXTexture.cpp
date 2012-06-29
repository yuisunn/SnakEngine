#include <SED3D9/DXTexture.h>
#include <SE3DEngine/Image/GutDDS.h>
#include <SE3DEngine/Image/GutImage.h>
#include <SE3DEngine/Image/GutBMP.h>
#include <SE3DEngine/Image/GutTGA.h>
#include <SE3DEngine/DeviceLayer/IDeviceLayer.h>

#pragma warning(disable: 4018)
#pragma warning(disable: 4996)



namespace SE
{
	//2d
    //读取dds文件 
    bool DXTexture::LoadDDS(const char *filename)
	{
		m_pD3DDevice = GetDevice();

  		//读取文件
		DDS_IMAGE_DATA *pDDSImageData = loadDDSTextureFile(filename);
	
		if ( pDDSImageData==NULL )
		{
			return false;
		}
	
		m_nHeight      = pDDSImageData->height;
		m_nWidth       = pDDSImageData->width;
		m_nMappedLevel = pDDSImageData->numMipMaps;

		m_pD3DDevice->CreateTexture(m_nWidth, m_nHeight, m_nMappedLevel, 0, (D3DFORMAT)pDDSImageData->format, D3DPOOL_MANAGED, &m_pD3DTexture2D, NULL);

		int nSize;
		int nOffset = 0;
		int nHeight = m_nHeight;
		int nWidth = m_nWidth;
		// DXn format
		if ( pDDSImageData->bBlock )
		{
			int nBlockSize;

			if( pDDSImageData->format == D3DFMT_DXT1 )
			{
				nBlockSize = 8;
			}
			else
			{
				nBlockSize = 16;
			}

			// 更mipmap瓜糷
		
			for( int i = 0; i < m_nMappedLevel; ++i )
			{
				if( m_nWidth  == 0 ) m_nWidth  = 1;
				if( m_nHeight == 0 ) m_nHeight = 1;

				nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;
			
				D3DLOCKED_RECT locked_rect;
				m_pD3DTexture2D->LockRect(i, &locked_rect, NULL, 0);
			
				memcpy(locked_rect.pBits, pDDSImageData->pixels + nOffset, nSize);
			
				m_pD3DTexture2D->UnlockRect(i);

				nOffset += nSize;
				// Half the image size for the next mip-map level...
				nWidth  = (nWidth  / 2);
				nHeight = (nHeight / 2);
			}
		}
		// DDS纗⊿Τ溃罽Α
		else
		{
			for( int i = 0; i < m_nMappedLevel; ++i )
			{
				if( nWidth  == 0 ) nWidth  = 1;
				if( nHeight == 0 ) nHeight = 1;

				nSize = nWidth * nHeight * pDDSImageData->PixelSize;

				D3DLOCKED_RECT locked_rect;
				m_pD3DTexture2D->LockRect(i, &locked_rect, NULL, 0);

				memcpy(locked_rect.pBits, pDDSImageData->pixels + nOffset, nSize);

				m_pD3DTexture2D->UnlockRect(i);

				nOffset += nSize;
				// Half the image size for the next mip-map level...
				nWidth  = (nWidth  / 2);
				nHeight = (nHeight / 2);
			}
		}

		if( pDDSImageData->pixels != NULL )
		{
			free( pDDSImageData->pixels );
		}

		free( pDDSImageData );
		return true;
	} 
	//读取tga和 bmp
	bool  DXTexture::LoadBMP(const char *filename, bool mipmap_enabled)
	{
		m_pD3DDevice =  GetDevice();
		char file_ext[16];
		_splitpath(filename, NULL, NULL, NULL, file_ext);
		strlwr(file_ext);

		GutImage *pLoader = NULL;

		if ( !strcmp(file_ext, ".tga") )
		{
			pLoader = new TGAImg;
		}
		else if ( !strcmp(file_ext, ".bmp") )
		{
			pLoader = new BMPImg;
		}
		else
		{
			return false;
		}

		if ( IMG_OK!=pLoader->Load(filename) )
		{
			return false;
		}

		int bytes_per_pixel = pLoader->GetBPP()/8;

		m_nWidth = pLoader->GetWidth();
		m_nHeight = pLoader->GetHeight();
		int p = m_nWidth > m_nHeight ? m_nWidth : m_nHeight;

		if ( m_nWidth==0 || m_nHeight==0 )
			return false;

		int mipmap_levels = 1;
		if ( mipmap_enabled )
		{
			int mask = 0x01;
			for ( int i=0; i<16; i++, mipmap_levels++, mask<<=1 )
			{
				if ( p & mask )
					break;
			}
		}

		// 玻ネ禟瓜ン
		if ( D3D_OK!=m_pD3DDevice->CreateTexture(m_nWidth, m_nHeight, 
			mipmap_levels, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pD3DTexture2D, NULL) )
			return false;
		m_nMappedLevel = mipmap_levels;
		for (int i=0; i<mipmap_levels; i++)
		{
			// ㄣ禟瓜戈
			D3DLOCKED_RECT locked_rect;
			m_pD3DTexture2D->LockRect(i, &locked_rect, NULL, 0);

			unsigned char *target = (unsigned char *)locked_rect.pBits;
			unsigned char *source = pLoader->GetImg();

			for ( int y=0; y<pLoader->GetHeight(); y++ )
			{
				for ( int x=0; x<pLoader->GetWidth(); x++ )
				{
					switch(bytes_per_pixel)
					{
					case 1:
						target[0] = source[0];
						target[1] = source[0];
						target[2] = source[0];
						target[3] = source[0];
						source++;
						break;
					case 3:
						target[0] = source[2];
						target[1] = source[1];
						target[2] = source[0];
						target[3] = 255;
						source +=3;
						break;
					case 4:
						target[0] = source[2];
						target[1] = source[1];
						target[2] = source[0];
						target[3] = source[3];
						source +=4;
						break;
					}

					target+=4;
				}
			}

			m_pD3DTexture2D->UnlockRect(i);
		
			if ( !pLoader->DownSampling() )
				break;
		}
	
		delete pLoader;
		return true;
	}
	
}
//cube
namespace SE
{
	//cube
	bool DXTextureCube::LoadCube(const char *filename)
	{
		//m_pD3DDevice = GetDevice();
		//D3DXCreateCubeTextureFromFileEx(m_pD3DDevice, filename, 
		//	0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		//	D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pD3DTextureCube);
		return true;
	}

}