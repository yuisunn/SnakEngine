#ifndef _D3D11Texture_H_
#define _D3D11Texture_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Image/CTexture.h>

namespace SE
{
	class _DLL_API D3D11Texture : public CTexture
	{
	public:
		D3D11Texture();
		~D3D11Texture();
		virtual const ETextureType GetTextureType() const;
		virtual void CopyToTexture(CTexture& pTarget);
		virtual void CopyToSubTexture1D(CTexture& pTarget,uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_Width,uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_Width);
		virtual void CopyToSubTexture2D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height);
		virtual void CopyToSubTexture3D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 dst_z_offset, uint32 uTar_Width, uint32 uTar_Height, uint32 uTar_Depth,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 src_z_offset, uint32 uSrc_Width, uint32 uSrc_Height, uint32 uSrc_Depth);
		virtual void CopyToSubTextureCube(CTexture& pTarget,
			uint32 uTar_ArrayIndex, ECubeFaces eTar_Face, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, ECubeFaces eSrc_Face, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height);
		//(void*)&
		virtual void Map1D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uWidth,
			void*& prData);
		virtual void Map2D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
			void*& prData, uint32& urRowPitch);
		virtual void Map3D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uZOffset,
			uint32 uWidth, uint32 height, uint32 depth,
			void*& prData, uint32& row_pitch, uint32& urSlicePitch);
		virtual void MapCube(uint32 uArrayIndex, ECubeFaces eFace, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
			void*& prData, uint32& urRowPitch);
		virtual void UnMap1D(uint32 uArrayIndex, uint32 uLevel);
		virtual void UnMap2D(uint32 uArrayIndex, uint32 uLevel);
		virtual void UnMap3D(uint32 uArrayIndex, uint32 uLevel);
		virtual void UnMapCube(uint32 uArrayIndex, ECubeFaces eFace, uint32 uLevel);
	    virtual bool SetTexture() ;
		virtual const String GetName() const ;
		virtual uint32 GetWidth(uint32 uLevel) const;
		virtual uint32 GetHeight(uint32 uLevel) const;
		virtual uint32 GetDepth(uint32 uLevel) const;
		virtual const int GetSourceWidth() const ;
		virtual const int GetSourceHeight() const ;
		virtual const int GetTextureID() const ;
		virtual const uint32 GetFlags() const ;
		virtual const int GetNumMips() const ;
		virtual const int GetPriority() const ;
		virtual const ETextureType GetAccessType() const ;
		virtual bool LoadTGA(const char *szName, bool bMips=false) ;
		virtual bool LoadBMP(const char *szName, bool bMips=false) ;
		virtual bool LoadDDS(const char *szName, bool bMips=false);
		virtual bool SaveDDS(const char *szName, bool bMips=true) ;
	private:
		ID3D11DevicePtr m_pDevice;
		ID3D11DeviceContextPtr m_pContext;
		std::vector<std::pair<D3D11_SHADER_RESOURCE_VIEW_DESC, ID3D11ShaderResourceViewPtr> > m_ShadResViews;
		std::vector<std::pair<D3D11_UNORDERED_ACCESS_VIEW_DESC, ID3D11UnorderedAccessViewPtr> > m_UnorAccesViews;


	};
	class _DLL_API D3D11Texture2D : public D3D11Texture
	{
	public:
		D3D11Texture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMipMap, uint32 uArraySize, PixelFormat Format, uint32 uSampleCount, uint32 uQuality, uint32 uAcces, PixelInitData const* cpInitData);
		~D3D11Texture2D();

		uint32 GetWidth(uint32 uLevel) const;
		uint32 GetHeight(uint32 uLevel) const;

	    void CopyToTexture(CTexture& pTarget);

		void CopyToSubTexture2D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height);
	    ID3D11ShaderResourceViewPtr const & RetriveD3DShaderResourceView(uint32 uFirstArrayIndex, uint32 uNumItems, uint32 uFirstLevel, uint32 uNumLevels);

		ID3D11UnorderedAccessViewPtr const & RetriveD3DUnorderedAccessView(uint32 uFirstArrayIndex, uint32 uNumItems, uint32 uLevel);

		ID3D11RenderTargetViewPtr const & RetriveD3DRenderTargetView(uint32 uFirstArrayIndex, uint32 uArraySize, uint32 uLevel);
		ID3D11DepthStencilViewPtr const & RetriveD3DDepthStencilView(uint32 uFirstArrayIndex, uint32 uArraySize, uint32 uLevel);
	    void BuildMipSubLevels();

		ID3D11ResourcePtr D3DResource() const
		{
			return m_pTex2D;
		}
		ID3D11Texture2DPtr const & D3DTexture() const
		{
			return m_pTex2D;
		}
	private:
		void Map2D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
			void*& prData, uint32& urRowPitch);
		void UnMap2D(uint32 uArrayIndex, uint32 uLevel);
		void UpdateParams();
	
		D3D11_TEXTURE2D_DESC m_Desc;
		ID3D11Texture2DPtr m_pTex2D;

		std::vector<uint32>	m_uWidthes;
		std::vector<uint32>	m_uHeights;
	};

	
}

#endif