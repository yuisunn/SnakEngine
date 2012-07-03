#ifndef _OGLESTexture_H_
#define _OGLESTexture_H_
//#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <map>

#include <SE3DEngine/Image/CTexture.h>
#include <SE3DEngine/Comm/Def.h>
#include <OpenglES/OGLESMapping.h>


namespace SE
{
	class OGLESTexture : public CTexture
	{
	public:
		OGLESTexture(ETextureType eType,uint32 uArraySize, uint32 uSample, uint32 uQuality, uint32 uAccess);
		~OGLESTexture();
		virtual const ETextureType GetTextureType() const;
		virtual void CopyToTexture(CTexture& pTarget);
		virtual void CopyToSubTexture1D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_Width,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_Width);
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

		const GLuint GetType() const;
		void BuildMipSubLevels();
		void BindTexture();
		void TexParameteri(GLenum eName, GLint iParam);
		void TexParameterf(GLenum EName, GLfloat fParam);
		GLenum GetGLTexType() const
		{
			return m_eGLTexType;
		}
		GLuint GetGLTex() const
		{
			return m_iGLTex;
		}


	protected:
		GLuint m_iGLTex;
		GLenum m_eGLTexType;
		ETextureAccessType m_eAccessType;
		std::vector<std::vector<uint8> > m_uTexDatas;

		std::map<GLenum, GLint> m_TexParamIs;
		std::map<GLenum, GLfloat> m_TexParamFs;
	};

	class OGLESTexture2D : public OGLESTexture
	{
	public:
		OGLESTexture2D(uint32 uWidth, uint32 uHeight, uint32 uNumMip, uint32 uArraySize, PixelFormat Format,
			uint32 uSampleCount, uint32 uSampleQuality, uint32 uAccess, PixelInitData const * cpInitData);
		~OGLESTexture2D();

		uint32 GetWidth(uint32 uLevel) const;
		uint32 GetHeight(uint32 uLevel) const;

	    void CopyToTexture(CTexture& pTarget);

		void CopyToSubTexture2D(CTexture& pTarget,
			uint32 uTar_ArrayIndex, uint32 uTar_Level, uint32 uTar_XOffset, uint32 uTar_YOffset, uint32 uTar_Width, uint32 uTar_Height,
			uint32 uSrc_ArrayIndex, uint32 uSrc_Level, uint32 uSrc_XOffset, uint32 uSrc_YOffset, uint32 uSrc_Width, uint32 uSrc_Height);
	private:
		void Map2D(uint32 uArrayIndex, uint32 uLevel, ETextureAccessType eAcessType,
			uint32 uXOffset, uint32 uYOffset, uint32 uWidth, uint32 uHeight,
			void*& prData, uint32& urRowPitch);
		void UnMap2D(uint32 uArrayIndex, uint32 uLevel);

	private:
		std::vector<uint32> m_uWidthes;
		std::vector<uint32> m_uHeights;
	};


}

#endif