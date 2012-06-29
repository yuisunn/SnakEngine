#ifndef _CTexture_H_
#define _CTexture_H_

#include <string>
#include <vector>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/PixelFormat.h>

namespace SE
{
		//贴图类型
	enum ETextureType
   {
			TT_1D,
			TT_2D,
			TT_3D,
			TT_Cube
	};
	enum ECubeFaces
	{
		CF_Positive_X = 0,
		CF_Negative_X = 1,
		CF_Positive_Y = 2,
		CF_Negative_Y = 3,
		CF_Positive_Z = 4,
		CF_Negative_Z = 5
	};

	enum ETextureAccessType
	{
		TAT_Read_Only,
		TAT_Write_Only,
		TAT_Read_Write
	};
	//贴图数据结构
	struct TexDesc
	{
		String sRes_name;

		uint32 uAccessHint;
		ETextureType eType;
		uint32 uWidth, uHeight, uDepth;
		uint32 uNumMipmap;
		uint32 uArraySize;
		PixelFormat format;
		std::vector<PixelInitData> initDatas;
		std::vector<uint8> dataBlocks;
 
	};
	//贴图基类
	class DLL_API CTexture 
	{
	public:	
		class Mapper : boost::noncopyable
		{
			friend class CTexture;
			//貌似用来除湿话 ctexture
		public:
			Mapper(CTexture& pTex, uint32 uArrayIndex, uint32_t uLevel, ETextureAccessType eAccType,
						uint32 uXOffset, uint32 uWidth)
				: m_pTex(pTex),
					m_uMappedArrayIndex(uArrayIndex),
					m_uMappedLevel(uLevel)
			{
				m_pTex.Map1D(uArrayIndex, uLevel, eAccType, uXOffset, uWidth, m_pData);
				m_uRowPitch = m_uSlicePitch = uWidth * NumFormatBytes(pTex.GetFormat());
			}
			Mapper(CTexture& pTex, uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAccType,
						uint32 uXOffset, uint32 uYOffset,
						uint32 uWidth, uint32 uHeight)
				: m_pTex(pTex),
					m_uMappedArrayIndex(uArrayIndex),
					m_uMappedLevel(uLevel)
			{
				m_pTex.Map2D(uArrayIndex, uLevel, eAccType, uXOffset, uYOffset, uWidth, uHeight, m_pData, m_uRowPitch);
				m_uSlicePitch = m_uRowPitch * uHeight;
			}
			Mapper(CTexture& pTex, uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAccType,
						uint32 uXOffset, uint32 uYOffset, uint32 z_offset,
						uint32 uWidth, uint32 uHeight, uint32 uDepth)
				: m_pTex(pTex),
					m_uMappedArrayIndex(uArrayIndex),
					m_uMappedLevel(uLevel)
			{
				m_pTex.Map3D(uArrayIndex, uLevel, eAccType, uXOffset, uYOffset, z_offset, uWidth, uHeight, uDepth, m_pData, m_uRowPitch, m_uSlicePitch);
			}
			Mapper(CTexture& pTex, uint32 uArrayIndex, ECubeFaces eFace, uint32 uLevel, ETextureAccessType eAccType,
						uint32 uXOffset, uint32 uYOffset,
						uint32 uWidth, uint32 uHeight)
				: m_pTex(pTex),
					m_uMappedArrayIndex(uArrayIndex),
					m_eMappedFace(eFace),
					m_uMappedLevel(uLevel)
			{
				m_pTex.MapCube(uArrayIndex, eFace, uLevel, eAccType, uXOffset, uYOffset, uWidth, uHeight, m_pData, m_uRowPitch);
				m_uSlicePitch = m_uRowPitch * uHeight;
			}

			~Mapper()
			{
				switch (m_pTex.GetType())
				{
				case TT_1D:
					m_pTex.UnMap1D(m_uMappedArrayIndex, m_uMappedLevel);
					break;

				case TT_2D:
					m_pTex.UnMap2D(m_uMappedArrayIndex, m_uMappedLevel);
					break;

				case TT_3D:
					m_pTex.UnMap3D(m_uMappedArrayIndex, m_uMappedLevel);
					break;

				case TT_Cube:
					m_pTex.UnMapCube(m_uMappedArrayIndex, m_eMappedFace, m_uMappedLevel);
					break;
				}
			}

			template <typename T>
			const T* Pointer() const
			{
				return static_cast<T*>(m_pData);
			}
			template <typename T>
			T* Pointer()
			{
				return static_cast<T*>(m_pData);
			}

			uint32_t RowPitch() const
			{
				return m_uRowPitch;
			}

			uint32_t SlicePitch() const
			{
				return m_uSlicePitch;
			}

		private:
			CTexture& m_pTex;

			void* m_pData;
			uint32 m_uRowPitch, m_uSlicePitch;

			uint32 m_uMappedArrayIndex;
			ECubeFaces m_eMappedFace;
			uint32 m_uMappedLevel;
		};

		explicit CTexture(ETextureType eType, uint32 uSample, uint32 uQuality, uint32 uAccess);
		virtual ~CTexture();
		virtual bool SetTexture() = 0;
		virtual const String GetName() const = 0;
		virtual uint32 GetWidth(uint32 uLevel) const = 0;
		virtual uint32 GetHeight(uint32 uLevel) const= 0;
		virtual uint32 GetDepth(uint32 uLevel) const = 0;
		virtual const int GetSourceWidth() const = 0;
		virtual const int GetSourceHeight() const = 0;
		virtual const int GetTextureID() const = 0;
		virtual const uint32 GetFlags() const = 0;
		virtual const int GetNumMips() const = 0;
		virtual const int GetPriority() const = 0;
		virtual const ETextureType GetTextureType() const = 0;
		virtual void CopyToTexture(CTexture& pTarget) = 0;
		virtual void CopyToSubTexture1D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_Width,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_Width) = 0;
		virtual void CopyToSubTexture2D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height) = 0;
		virtual void CopyToSubTexture3D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 dst_z_offset, uint32 uTar_Width, uint32 uTar_Height, uint32 uTar_Depth,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 src_z_offset, uint32 uSrc_Width, uint32 uSrc_Height, uint32 uSrc_Depth) = 0;
		virtual void CopyToSubTextureCube(CTexture& pTarget,
			uint32 uTar_ArrayIndex, ECubeFaces eTar_Face, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, ECubeFaces eSrc_Face, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height) = 0;

		virtual void BuildMipSubLevels() = 0;
		//(void*)&
		virtual void Map1D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uWidth,
			void*& prData) = 0;
		virtual void Map2D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
			void*& prData, uint32& urRowPitch) = 0;
		virtual void Map3D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uZOffset,
			uint32 uWidth, uint32 uHeight, uint32 depth,
			void*& prData, uint32& row_pitch, uint32& urSlicePitch) = 0;
		virtual void MapCube(uint32 uArrayIndex, ECubeFaces eFace, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
			void*& prData, uint32& urRowPitch) = 0;

		virtual void UnMap1D(uint32 uArrayIndex, uint32 uLevel) = 0;
		virtual void UnMap2D(uint32 uArrayIndex, uint32 uLevel) = 0;
		virtual void UnMap3D(uint32 uArrayIndex, uint32 uLevel) = 0;
		virtual void UnMapCube(uint32 uArrayIndex, ECubeFaces eFace, uint32 uLevel) = 0;

		uint32 GetNumMip() const;
		uint32 GetArraySize() const;
		PixelFormat GetFormat() const;
		ETextureType GetType() const;
		uint32 GetSampleCount() const;
		uint32 GetSampleQuality() const;
		uint32 GetAccess() const;


	protected:
		int32 m_iNumMip;
		int32 m_iArraySize;
		PixelFormat m_Format;
		ETextureType m_eTexType;
		uint32 m_uSample;
		uint32 m_uSampleQuality;
		uint32 m_uAccess;

	};

}

#endif