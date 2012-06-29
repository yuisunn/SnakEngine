#ifndef _D3D9Texture_H_
#define _D3D9Texture_H_

#include <SE3DEngine/Image/CTexture.h>
#include <SE3DEngine/Comm/Def.h>
#include <string>
#include <SE3DEngine/ResourceSystem/IResource.h>
#include <SE3DEngine/Comm/PixelFormat.h>



namespace SE
{

	// 载入DDS格式文件
	void LoadTexture(String const & sTexName, ETextureType& type,
		uint32& uWidth, uint32& uHeight, uint32& uDepth, uint32_t& numMipMaps, uint32& uArraySize,
		PixelFormat& format, std::vector<ElementInitData>& initData, std::vector<uint8>& uDataBlocks)
	{
		
		//确认是否dds
		uint32 magic;
		file->read(&magic, sizeof(magic));
		LittleEndianToNative<sizeof(magic)>(&magic);
		BOOST_ASSERT((MakeFourCC<'D', 'D', 'S', ' '>::value) == magic);

		//读取文件头信息
		DDS_HEADER desc;
		file->read(&desc, sizeof(desc));
		LittleEndianToNative<sizeof(desc.uSize)>(&desc.uSize);
		LittleEndianToNative<sizeof(desc.uFlags)>(&desc.uFlags);
		LittleEndianToNative<sizeof(desc.uHeight)>(&desc.uHeight);
		LittleEndianToNative<sizeof(desc.uWidth)>(&desc.uWidth);
		LittleEndianToNative<sizeof(desc.iPitch)>(&desc.iPitch);
		LittleEndianToNative<sizeof(desc.uDepth)>(&desc.uDepth);
		LittleEndianToNative<sizeof(desc.uMip_map_count)>(&desc.uMip_map_count);
		for (uint32 i = 0; i < sizeof(desc.uReserved2) / sizeof(desc.uReserved2[0]); ++ i)
		{
			LittleEndianToNative<sizeof(desc.ugReserved1[i])>(&desc.ugReserved1[i]);
		}
		//像素格式
		LittleEndianToNative<sizeof(desc.pixelFormat.uSize)>(&desc.pixelFormat.uSize);
		LittleEndianToNative<sizeof(desc.pixelFormat.uFlags)>(&desc.pixelFormat.uFlags);
		LittleEndianToNative<sizeof(desc.pixelFormat.uFourCC)>(&desc.pixelFormat.uFourCC);
		LittleEndianToNative<sizeof(desc.pixelFormat.uRGB_bit_count)>(&desc.pixelFormat.uRGB_bit_count);
		LittleEndianToNative<sizeof(desc.pixelFormat.uR_bit_mask)>(&desc.pixelFormat.uR_bit_mask);
		LittleEndianToNative<sizeof(desc.pixelFormat.uG_bit_mask)>(&desc.pixelFormat.uG_bit_mask);
		LittleEndianToNative<sizeof(desc.pixelFormat.uB_bit_mask)>(&desc.pixelFormat.uB_bit_mask);
		LittleEndianToNative<sizeof(desc.pixelFormat.uRGB_alpha_bit_mask)>(&desc.pixelFormat.uRGB_alpha_bit_mask);
		LittleEndianToNative<sizeof(desc.ddsCaps.uCaps1)>(&desc.ddsCaps.uCaps1);
		LittleEndianToNative<sizeof(desc.ddsCaps.uCaps2)>(&desc.ddsCaps.uCaps2);
		for (uint32_t i = 0; i < sizeof(desc.ddsCaps.ugReserved) / sizeof(desc.ddsCaps.ugReserved[0]); ++ i)
		{
			LittleEndianToNative<sizeof(desc.ddsCaps.ugReserved[i])>(&desc.ddsCaps.ugReserved[i]);
		}
		LittleEndianToNative<sizeof(desc.uReserved2)>(&desc.uReserved2);

		DDS_HEADER_DXT10 desc10;
		if (MakeFourCC<'D', 'X', '1', '0'>::value == desc.pixelFormat.uFourCC)
		{
			file->read(&desc10, sizeof(desc10));
			LittleEndianToNative<sizeof(desc10.uDXgiFormat)>(&desc10.uDXgiFormat);
			LittleEndianToNative<sizeof(desc10.eResourceDim)>(&desc10.eResourceDim);
			LittleEndianToNative<sizeof(desc10.uMiscFlag)>(&desc10.uMiscFlag);
			LittleEndianToNative<sizeof(desc10.uArraySize)>(&desc10.uArraySize);
			LittleEndianToNative<sizeof(desc10.uReserved)>(&desc10.uReserved);
			array_size = desc10.uArraySize;
		}
		else
		{
			std::memset(&desc10, 0, sizeof(desc10));
			array_size = 1;
	
			BOOST_ASSERT((desc.uFlags & DDSD_CAPS) != 0);
			BOOST_ASSERT((desc.uFlags & DDSD_PIXELFORMAT) != 0);
		}

		BOOST_ASSERT((desc.uFlags & DDSD_WIDTH) != 0);
		BOOST_ASSERT((desc.uFlags & DDSD_HEIGHT) != 0);

		if (0 == (desc.uFlags & DDSD_MIPMAPCOUNT))
		{
			desc.uMip_map_count = 1;
		}
		//贴图格式获得
		format = EF_ARGB8;
		if ((desc.pixelFormat.uFlags & DDSPF_FOURCC) != 0)
		{
			// From "Programming Guide for DDS", http://msdn.microsoft.com/en-us/library/bb943991.aspx
			switch (desc.pixelFormat.uFourCC)
			{
			case 36:
				format = EF_ABGR16;
				break;

			case 110:
				format = EF_SIGNED_ABGR16;
				break;

			case 111:
				format = EF_R16F;
				break;

			case 112:
				format = EF_GR16F;
				break;

			case 113:
				format = EF_ABGR16F;
				break;

			case 114:
				format = EF_R32F;
				break;

			case 115:
				format = EF_GR32F;
				break;

			case 116:
				format = EF_ABGR32F;
				break;

			case MakeFourCC<'D', 'X', 'T', '1'>::value:
				format = EF_BC1;
				break;

			case MakeFourCC<'D', 'X', 'T', '3'>::value:
				format = EF_BC2;
				break;

			case MakeFourCC<'D', 'X', 'T', '5'>::value:
				format = EF_BC3;
				break;

			case MakeFourCC<'B', 'C', '4', 'U'>::value:
			case MakeFourCC<'A', 'T', 'I', '1'>::value:
				format = EF_BC4;
				break;

			case MakeFourCC<'B', 'C', '4', 'S'>::value:
				format = EF_SIGNED_BC4;
				break;

			case MakeFourCC<'A', 'T', 'I', '2'>::value:
				format = EF_BC5;
				break;

			case MakeFourCC<'B', 'C', '5', 'S'>::value:
				format = EF_SIGNED_BC5;
				break;

			case MakeFourCC<'D', 'X', '1', '0'>::value:
				format = FromDXGIFormat(desc10.uDXgiFormat);
				break;
			}
		}
		else
		{
			if ((desc.pixelFormat.uFlags & DDSPF_RGB) != 0)
			{
				switch (desc.pixelFormat.uRGB_bit_count)
				{
				case 16:
					if ((0xF000 == desc.pixelFormat.uRGB_alpha_bit_mask)
						&& (0x0F00 == desc.pixelFormat.uR_bit_mask)
						&& (0x00F0 == desc.pixelFormat.uG_bit_mask)
						&& (0x000F == desc.pixelFormat.uB_bit_mask))
					{
						format = EF_ARGB4;
					}
					else
					{
						BOOST_ASSERT(false);
					}
					break;
					//无 aalpha
				case 32:
					if ((0x00FF0000 == desc.pixelFormat.uR_bit_mask)
						&& (0x0000FF00 == desc.pixelFormat.uG_bit_mask)
						&& (0x000000FF == desc.pixelFormat.uB_bit_mask))
					{
						if ((desc.pixelFormat.uFlags & DDSPF_ALPHAPIXELS) != 0)
						{
							format = EF_ARGB8;
						}
						else
						{
							BOOST_ASSERT(false);
						}
					}
					else
					{
						if ((0xC0000000 == desc.pixelFormat.uRGB_alpha_bit_mask)
							&& (0x3FF00000 == desc.pixelFormat.uR_bit_mask)
							&& (0x000FFC00 == desc.pixelFormat.uG_bit_mask)
							&& (0x000003FF == desc.pixelFormat.uB_bit_mask))
						{
							format = EF_A2BGR10;
						}
						else
						{
							if ((0xFF000000 == desc.pixelFormat.uRGB_alpha_bit_mask)
								&& (0x000000FF == desc.pixelFormat.uR_bit_mask)
								&& (0x0000FF00 == desc.pixelFormat.uG_bit_mask)
								&& (0x00FF0000 == desc.pixelFormat.uB_bit_mask))
							{
								format = EF_ABGR8;
							}
							else
							{
								if ((0x00000000 == desc.pixelFormat.uRGB_alpha_bit_mask)
									&& (0x0000FFFF == desc.pixelFormat.uR_bit_mask)
									&& (0xFFFF0000 == desc.pixelFormat.uG_bit_mask)
									&& (0x00000000 == desc.pixelFormat.uB_bit_mask))
								{
									format = EF_GR16;
								}
								else
								{
									BOOST_ASSERT(false);
								}
							}
						}
					}
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			else
			{
				if ((desc.pixelFormat.uFlags & DDSPF_LUMINANCE) != 0)
				{
					switch (desc.pixelFormat.uRGB_bit_count)
					{
					case 8:
						if (0 == (desc.pixelFormat.uFlags & DDSPF_ALPHAPIXELS))
						{
							format = EF_R8;
						}
						else
						{
							BOOST_ASSERT(false);
						}
						break;

					case 16:
						if (0 == (desc.pixelFormat.uFlags & DDSPF_ALPHAPIXELS))
						{
							format = EF_R16;
						}
						else
						{
							BOOST_ASSERT(false);
						}
						break;

					default:
						BOOST_ASSERT(false);
						break;
					}
				}
				else
				{
					if ((desc.pixelFormat.uFlags & DDSPF_BUMPDUDV) != 0)
					{
						switch (desc.pixelFormat.uRGB_bit_count)
						{
						case 16:
							if ((0x000000FF == desc.pixelFormat.uR_bit_mask)
								&& (0x0000FF00 == desc.pixelFormat.uG_bit_mask))
							{
								format = EF_SIGNED_GR8;
							}
							else
							{
								if (0x0000FFFF == desc.pixelFormat.uR_bit_mask)
								{
									format = EF_SIGNED_R16;
								}
								else
								{
									BOOST_ASSERT(false);
								}
							}
							break;

						case 32:
							if ((0x000000FF == desc.pixelFormat.uR_bit_mask)
								&& (0x0000FF00 == desc.pixelFormat.uG_bit_mask)
								&& (0x00FF0000 == desc.pixelFormat.uB_bit_mask))
							{
								format = EF_SIGNED_ABGR8;
							}
							else
							{
								if ((0xC0000000 == desc.pixelFormat.uRGB_alpha_bit_mask)
									&& (0x3FF00000 == desc.pixelFormat.uR_bit_mask)
									&& (0x000FFC00 == desc.pixelFormat.uG_bit_mask)
									&& (0x000003FF == desc.pixelFormat.uB_bit_mask))
								{
									format = EF_SIGNED_A2BGR10;
								}
								else
								{
									if ((0x00000000 == desc.pixelFormat.uRGB_alpha_bit_mask)
										&& (0x0000FFFF == desc.pixelFormat.uR_bit_mask)
										&& (0xFFFF0000 == desc.pixelFormat.uG_bit_mask)
										&& (0x00000000 == desc.pixelFormat.uB_bit_mask))
									{
										format = EF_SIGNED_GR16;
									}
									else
									{
										BOOST_ASSERT(false);
									}
								}
							}
							break;

						default:
							BOOST_ASSERT(false);
							break;
						}
					}
					else
					{
						if ((desc.pixelFormat.uFlags & DDSPF_ALPHA) != 0)
						{
							format = EF_A8;
						}
						else
						{
							BOOST_ASSERT(false);
						}
					}
				}
			}
		}
		//贴图根据 flag 获得可用的头文件信息
		uint32 main_image_size;
		if ((desc.uFlags & DDSD_LINEARSIZE) != 0)
		{
			main_image_size = desc.uLinearSize;
		}
		else
		{
			if ((desc.uFlags & DDSD_PITCH) != 0)
			{
				main_image_size = desc.iPitch * desc.uHeight;
			}
			else
			{
				if ((desc.uFlags & desc.pixelFormat.uFlags & 0x00000040) != 0)
				{
					main_image_size = desc.uWidth * desc.uHeight * desc.pixelFormat.uRGB_bit_count / 8;
				}
				else
				{
					main_image_size = desc.uWidth * desc.uHeight * NumFormatBytes(format);
				}
			}
		}

		if (desc.ugReserved1[0] != 0)
		{
			format = MakeSRGB(format);
		}

		width = desc.uWidth;
		numMipMaps = desc.uMip_map_count;

		if (MakeFourCC<'D', 'X', '1', '0'>::value == desc.pixelFormat.uFourCC)
		{
			if (D3D10_RESOURCE_MISC_TEXTURECUBE == desc10.uMiscFlag)
			{
				type = TT_Cube;
				array_size /= 6;
				height = desc.uWidth;
				depth = 1;
			}
			else
			{
				switch (desc10.eResourceDim)
				{
				case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
					type = TT_1D;
					height = 1;
					depth = 1;
					break;

				case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
					type = TT_2D;
					height = desc.uHeight;
					depth = 1;
					break;

				case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
					type = TT_3D;
					height = desc.uHeight;
					depth = desc.uDepth;
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}	
		}
		else
		{
			if ((desc.ddsCaps.uCaps2 & DDSCAPS2_CUBEMAP) != 0)
			{
				type = TT_Cube;
				height = desc.uWidth;
				depth = 1;
			}
			else
			{
				if ((desc.ddsCaps.uCaps2 & DDSCAPS2_VOLUME) != 0)
				{
					type = TT_3D;
					height = desc.uWidth;
					depth = desc.uDepth;
				}
				else
				{
					type = TT_2D;
					height = desc.uHeight;
					depth = 1;
				}
			}
		}
        //根据贴图  mip 信息
		std::vector<size_t> base;
		uint32 format_size = NumFormatBytes(format);
		switch (type)
		{
		case T_1D:
			{
				init_data.resize(array_size * numMipMaps);
				base.resize(array_size * numMipMaps);
				for (uint32 array_index = 0; array_index < array_size; ++ array_index)
				{
					uint32 the_width = width;
					for (uint32 level = 0; level < numMipMaps; ++ level)
					{
						size_t const index = array_index * numMipMaps + level;
						uint32 image_size;
						if (IsCompressedFormat(format))
						{
							int block_size;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format)
								|| (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
							{
								block_size = 8;
							}
							else
							{
								block_size = 16;
							}

							image_size = ((the_width + 3) / 4) * block_size;
						}
						else
						{
							image_size = main_image_size / (1UL << (level * 2));
						}

						base[index] = data_block.size();
						data_block.resize(base[index] + image_size);
						init_data[index].row_pitch = image_size;
						init_data[index].slice_pitch = image_size;

						file->read(&data_block[base[index]], static_cast<std::streamsize>(image_size));
						BOOST_ASSERT(file->gcount() == static_cast<int>(image_size));

						the_width = std::max<uint32>(the_width / 2, 1);
					}
				}
			}
			break;

		case Texture::TT_2D:
			{
				init_data.resize(array_size * numMipMaps);
				base.resize(array_size * numMipMaps);
				for (uint32_t array_index = 0; array_index < array_size; ++ array_index)
				{
					uint32_t the_width = width;
					uint32_t the_height = height;
					for (uint32_t level = 0; level < numMipMaps; ++ level)
					{
						size_t const index = array_index * numMipMaps + level;
						if (IsCompressedFormat(format))
						{
							int block_size;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format)
								|| (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
							{
								block_size = 8;
							}
							else
							{
								block_size = 16;
							}

							uint32_t image_size = ((the_width + 3) / 4) * ((the_height + 3) / 4) * block_size;

							base[index] = data_block.size();
							data_block.resize(base[index] + image_size);
							init_data[index].row_pitch = (the_width + 3) / 4 * block_size;
							init_data[index].slice_pitch = image_size;

							file->read(&data_block[base[index]], static_cast<std::streamsize>(image_size));
							BOOST_ASSERT(file->gcount() == static_cast<int>(image_size));
						}
						else
						{
							if (desc.uFlags & DDSD_PITCH)
							{
								init_data[index].row_pitch = static_cast<uint32_t>(((desc.iPitch >> level) + 3) / 4 * 4);
							}
							else
							{
								init_data[index].row_pitch = the_width * format_size;
							}
							init_data[index].slice_pitch = init_data[index].row_pitch * the_height;
							base[index] = data_block.size();
							data_block.resize(base[index] + init_data[index].slice_pitch);

							file->read(&data_block[base[index]], static_cast<std::streamsize>(init_data[index].slice_pitch));
						}

						the_width = std::max<uint32>(the_width / 2, 1);
						the_height = std::max<uint32>(the_height / 2, 1);
					}
				}
			}
			break;

		case Texture::TT_3D:
			{
				init_data.resize(array_size * numMipMaps);
				base.resize(array_size * numMipMaps);
				for (uint32_t array_index = 0; array_index < array_size; ++ array_index)
				{
					uint32_t the_width = width;
					uint32_t the_height = height;
					uint32_t the_depth = depth;
					for (uint32_t level = 0; level < numMipMaps; ++ level)
					{
						size_t const index = array_index * numMipMaps + level;
						if (IsCompressedFormat(format))
						{
							int block_size;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format)
								|| (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
							{
								block_size = 8;
							}
							else
							{
								block_size = 16;
							}

							uint32_t image_size = ((the_width + 3) / 4) * ((the_height + 3) / 4) * the_depth * block_size;

							base[index] = data_block.size();
							data_block.resize(base[index] + image_size);
							init_data[index].row_pitch = (the_width + 3) / 4 * block_size;
							init_data[index].slice_pitch = ((the_width + 3) / 4) * ((the_height + 3) / 4) * block_size;

							file->read(&data_block[base[index]], static_cast<std::streamsize>(image_size));
							BOOST_ASSERT(file->gcount() == static_cast<int>(image_size));
						}
						else
						{
							if (desc.uFlags & DDSD_PITCH)
							{
								init_data[index].row_pitch = static_cast<uint32_t>(((desc.iPitch >> level) + 3) / 4 * 4);
								init_data[index].slice_pitch = init_data[index].row_pitch * (the_height + 3) / 4 * 4;
							}
							else
							{
								init_data[index].row_pitch = the_width * format_size;
								init_data[index].slice_pitch = init_data[index].row_pitch * the_height;
							}
							base[index] = data_block.size();
							data_block.resize(base[index] + init_data[index].slice_pitch * the_depth);

							file->read(&data_block[base[index]], static_cast<std::streamsize>(init_data[index].slice_pitch * the_depth));
							BOOST_ASSERT(file->gcount() == static_cast<int>(init_data[index].slice_pitch * the_depth));
						}

						the_width = std::max<uint32>(the_width / 2, 1);
						the_height = std::max<uint32>(the_height / 2, 1);
						the_depth = std::max<uint32>(the_depth / 2, 1);
					}
				}
			}
			break;

		case Texture::TT_Cube:
			{
				init_data.resize(array_size * 6 * numMipMaps);
				base.resize(array_size * 6 * numMipMaps);
				for (uint32_t array_index = 0; array_index < array_size; ++ array_index)
				{
					for (uint32_t face = Texture::CF_Positive_X; face <= Texture::CF_Negative_Z; ++ face)
					{
						uint32_t the_width = width;
						uint32_t the_height = height;
						for (uint32_t level = 0; level < numMipMaps; ++ level)
						{
							size_t const index = (array_index * 6 + face - Texture::CF_Positive_X) * numMipMaps + level;
							if (IsCompressedFormat(format))
							{
								int block_size;
								if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format)
									|| (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
								{
									block_size = 8;
								}
								else
								{
									block_size = 16;
								}

								uint32_t image_size = ((the_width + 3) / 4) * ((the_height + 3) / 4) * block_size;

								base[index] = data_block.size();
								data_block.resize(base[index] + image_size);
								init_data[index].row_pitch = (the_width + 3) / 4 * block_size;
								init_data[index].slice_pitch = image_size;

								file->read(&data_block[base[index]], static_cast<std::streamsize>(image_size));
								BOOST_ASSERT(file->gcount() == static_cast<int>(image_size));
							}
							else
							{
								if (desc.uFlags & DDSD_PITCH)
								{
									init_data[index].row_pitch = static_cast<uint32_t>(((desc.iPitch >> level) + 3) / 4 * 4);
								}
								else
								{
									init_data[index].row_pitch = the_width * format_size;
								}
								init_data[index].slice_pitch = init_data[index].row_pitch * the_width;
								base[index] = data_block.size();
								data_block.resize(base[index] + init_data[index].slice_pitch);

								file->read(&data_block[base[index]], static_cast<std::streamsize>(init_data[index].slice_pitch));
								BOOST_ASSERT(file->gcount() == static_cast<int>(init_data[index].slice_pitch));
							}

							the_width = std::max<uint32>(the_width / 2, 1);
							the_height = std::max<uint32>(the_height / 2, 1);
						}
					}
				}
			}
			break;
		}

		for (size_t i = 0; i < base.size(); ++ i)
		{
			init_data[i].data = &data_block[base[i]];
		}
	}
	//2d
	class DXTexture :public ITexture, public IRes
	{
	public:
		explicit DXTexture();
		~DXTexture(void);

		bool LoadDDS(const char *filename);
		bool LoadBMP(const char *filename, bool mipmap_enabled);
		virtual bool SetTexture() = 0;
		virtual const char *GetName() const = 0;
		virtual const int GetWidth() const = 0;
		virtual const int GetHeight() const= 0;
		virtual const int GetDepth() const = 0;
		virtual const int GetSourceWidth() const = 0;
		virtual const int GetSourceHeight() const = 0;
		virtual const int GetTextureID() const = 0;
		virtual const uint32 GetFlags() const = 0;
		virtual const int GetNumMips() const = 0;
		virtual const int GetPriority() const = 0;
		virtual const ETextureType GetTextureType() const = 0;
		virtual bool LoadTGA(const char *szName, bool bMips=false) = 0;
		virtual bool SaveDDS(const char *szName, bool bMips=true) = 0;
	    virtual void GetMemoryUsage( ETextureAccess *tex_acess ) const = 0;

	private:
		LPDIRECT3DTEXTURE9 m_pD3DTexture2D;
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		D3DFORMAT m_Format;
		uint32 m_nWidth,m_nHeight;
		uint32 m_nMappedLevel;
		D3DPOOL m_Pool;
		uint32 m_nPriority;
	};


	//3d
	//cube
	
	//rendertarget
	//stencil

}


#endif

