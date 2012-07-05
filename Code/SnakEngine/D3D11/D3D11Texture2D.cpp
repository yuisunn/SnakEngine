#include <D3D11/D3D11Texture.h>

namespace SE
{
		D3D11Texture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMap, uint32 uArraySize, PixelFormat Format, uint32 uSampleCount, uint32 uQuality, uint32 uAcces, PixelInitData const* cpInitData)
			:D3D11Texture(TT_2D, uSampleCount, uQuality, uAcces)
		{
			if (0 == uNumMipMap)
		{
			num_mip_maps_ = 1;
			uint32 w = width;
			uint32 h = height;
			while ((w != 1) || (h != 1))
			{
				++ num_mip_maps_;

				w = std::max<uint32_t>(1U, w / 2);
				h = std::max<uint32_t>(1U, h / 2);
			}
		}
		else
		{
			m_iNumMip = numMipMaps;
		}

		D3D11Render const & re = *checked_cast<D3D11Render const *>(&MonFac::Only().InstFactory().InstRender());
		if (re.DeviceFeatureLevel() <= D3D_FEATURE_LEVEL_9_3)
		{
			if ((m_iNumMip > 1) && (((width & (width - 1)) != 0) || ((height & (height - 1)) != 0)))
			{
				// height or width is not a power of 2 and multiple mip levels are specified. This is not supported at feature levels below 10.0.
				m_iNumMip = 1;
			}
		}

		m_iArraySize = uArraySize;
		m_Format = Format;
		m_uWidthes.assign(1, width);
		m_uHeights.assign(1, height);

		m_Desc.Width = width;
		m_Desc.Height = height;
		m_Desc.MipLevels = num_mip_maps_;
		m_Desc.ArraySize = array_size_;
		switch (m_Format)
		{
		case EF_D16:
			desc_.Format = DXGI_FORMAT_R16_TYPELESS;
			break;

		case EF_D24S8:
			desc_.Format = DXGI_FORMAT_R24G8_TYPELESS;
			break;

		case EF_D32F:
			desc_.Format = DXGI_FORMAT_R32_TYPELESS;
			break;

		default:
			desc_.Format = D3D11Mapping::MappingFormat(format_);
			break;
		}
		desc_.SampleDesc.Count = sample_count;
		desc_.SampleDesc.Quality = sample_quality;

		this->GetD3DFlags(desc_.Usage, desc_.BindFlags, desc_.CPUAccessFlags, desc_.MiscFlags);

		std::vector<D3D11_SUBRESOURCE_DATA> subres_data(array_size_ * num_mip_maps_);
		if (init_data != NULL)
		{
			for (uint32_t j = 0; j < array_size_; ++ j)
			{
				for (uint32_t i = 0; i < num_mip_maps_; ++ i)
				{
					subres_data[j * num_mip_maps_ + i].pSysMem = init_data[j * num_mip_maps_ + i].data;
					subres_data[j * num_mip_maps_ + i].SysMemPitch = init_data[j * num_mip_maps_ + i].row_pitch;
					subres_data[j * num_mip_maps_ + i].SysMemSlicePitch = init_data[j * num_mip_maps_ + i].slice_pitch;
				}
			}
		}

		ID3D11Texture2D* d3d_tex;
		TIF(d3d_device_->CreateTexture2D(&desc_, (init_data != NULL) ? &subres_data[0] : NULL, &d3d_tex));
		d3dTexture2D_ = MakeCOMPtr(d3d_tex);

		this->UpdateParams();
		}
		~D3D11Texture2D::D3D11Texture2D();

		uint32 D3D11Texture2D::GetWidth(uint32 uLevel) const;
		uint32 D3D11Texture2D::GetHeight(uint32 uLevel) const;

	    void CopyToTexture(CTexture& pTarget);

		void CopyToSubTexture2D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height);
	    ID3D11ShaderResourceViewPtr const & RetriveD3DShaderResourceView(uint32 uFirstArrayIndex, uint32 uNumItems, uint32 uFirstLevel, uint32 uNumLevels);

		ID3D11UnorderedAccessViewPtr const & RetriveD3DUnorderedAccessView(uint32 uFirstArrayIndex, uint32 uNumItems, uint32 uLevel);

		ID3D11RenderTargetViewPtr const & RetriveD3DRenderTargetView(uint32 uFirstArrayIndex, uint32 uArraySize, uint32 uLevel);
		ID3D11DepthStencilViewPtr const & RetriveD3DDepthStencilView(uint32 uFirstArrayIndex, uint32 uArraySize, uint32 uLevel);
	    void BuildMipSubLevels();
		void Map2D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
		void*& prData, uint32& urRowPitch);
		void UnMap2D(uint32 uArrayIndex, uint32 uLevel);
		void UpdateParams();

}

