#ifndef _IShader_H_
#define _IShader_H_

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/SceneSystem/Camera.h>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Image/CTexture.h>
#include <SE3DEngine/RenderSystem/CGraphBuffer.h>

#pragma warning(disable: 4996)
#pragma warning(disable: 4018)

//shader 參數
namespace SE
{
   enum EShaderParamType
   {
	  SPT_UNKNOWN,
	  SPT_BYTE,
	  SPT_BOOL,
	  SPT_SHORT,
	  SPT_INT,
	  SPT_HALF,
	  SPT_FLOAT,
	  SPT_STRING,
	  SPT_FCOLOR,
	  SPT_VECTOR3,
	  SPT_MATRIX4X4,
	  SPT_CAMERA,
	  SPT_TEXTURECUBE,
	  SPT_TEXTURE2D,
	  SPT_TEXTURE3D,
	  SPT_SAMPLER
	};
   //union 和 struct  
 //  我们可以查看内存里面的分布：
	//类型         c     i    var    大小        
	//STest     0     4    12     20
	//UTest     0     0    0      8
   union UShaderParamVal
   {
	  byte m_Byte;
	  bool m_Bool;
	  short m_Short;
	  int m_Int;
	  float m_Float;
	  char* m_String;
	  float m_Color[4];
	  float m_Vector[3];
	  void* m_pTexture;
	  Matrix4x4* m_pMatrix4x4;

	  Camera* m_pCamera;
	};
   //shader参数设置
   struct SShaderParam
   {
	   char m_Name[32];
	   EShaderParamType m_Type;
	   UShaderParamVal m_Value;

	   inline void Construct()
	   {
		   m_Value.m_Int = 0;
		   m_Type = SPT_UNKNOWN;
		   m_Name[0] = 0;
	   }
	   inline SShaderParam()
	   {
	 	    Construct();
	   }
	   inline void Destroty()
	   {
		    if (m_Type == SPT_STRING)
               delete [] m_Value.m_String;
	   }
	   inline ~SShaderParam()
	   {
		   Destroty();
	   }
	   inline SShaderParam (const SShaderParam& src)
	   {
			memcpy(m_Name, src.m_Name, sizeof(m_Name));
			m_Type = src.m_Type;
			m_Value = src.m_Value;
	   }
	   inline SShaderParam& operator = (const SShaderParam& src)
	   {
			this->~SShaderParam();
			new(this) SShaderParam(src);
			return *this;
	   }
	   static bool SetParam(const char* name, std::vector<SShaderParam>* Params, UShaderParamVal& pr)
	   {
			uint32 i;
			for (i=0; i<(uint32)Params->size(); i++)
			{ 
			  SShaderParam *sp = &(*Params)[i]; 
			  if (!sp)
				continue;
			  if (!stricmp(sp->m_Name, name))
			  {
				switch (sp->m_Type)
				{          
				  case SPT_FLOAT:
					sp->m_Value.m_Float = pr.m_Float;
					break;
				  case SPT_SHORT:
					sp->m_Value.m_Short = pr.m_Short;
					break;
				  case SPT_INT:
				  case SPT_TEXTURE2D:
					sp->m_Value.m_Int = pr.m_Int;
					break;

				  case SPT_VECTOR3:
					sp->m_Value.m_Vector[0] = pr.m_Vector[0];
					sp->m_Value.m_Vector[1] = pr.m_Vector[1];
					sp->m_Value.m_Vector[2] = pr.m_Vector[2];
					break;
				  case SPT_FCOLOR:
					sp->m_Value.m_Color[0] = pr.m_Color[0];
					sp->m_Value.m_Color[1] = pr.m_Color[1];
					sp->m_Value.m_Color[2] = pr.m_Color[2];
					sp->m_Value.m_Color[3] = pr.m_Color[3];
					break;

				  case SPT_STRING:
					{
					  char *str = pr.m_String;
					  size_t len = strlen(str)+1;
					  sp->m_Value.m_String = new char [len];
					  strcpy(sp->m_Value.m_String, str);
					}
					break;
				}
				break;
			  }
			}
			if (i == Params->size())
			  return false;
			return true;
	   }
	   //
	   static bool GetValue(const char* szName, std::vector<SShaderParam> *Params, float *v, int nID)
	   {
		  int i;
		  bool bRes = false;
		  for (i=0; i<Params->size(); i++)
		  {
			SShaderParam *sp = &(*Params)[i];

			if (!sp)
			  continue;
			if (!stricmp(sp->m_Name, szName))
			{
			  bRes = true;
			  switch (sp->m_Type)
			  {
			  case SPT_HALF:
			  case SPT_FLOAT:
				v[nID] = sp->m_Value.m_Float;
				break;
			  case SPT_SHORT:
				v[nID] = (float)sp->m_Value.m_Short;
				break;
			  case SPT_INT:
			  case SPT_VECTOR3:
				v[0] = sp->m_Value.m_Vector[0];
				v[1] = sp->m_Value.m_Vector[1];
				v[2] = sp->m_Value.m_Vector[2];
				break;
			  case SPT_FCOLOR:
				v[0] = sp->m_Value.m_Color[0];
				v[1] = sp->m_Value.m_Color[1];
				v[2] = sp->m_Value.m_Color[2];
				v[3] = sp->m_Value.m_Color[3];
				break;
			  case SPT_STRING:
				assert(0);
				bRes = false;
				break;
			  }
			  break;
			}
		  }
		  return bRes;
		}


   };
}
//shader 
namespace SE
{
	//shader 类型 vs ps =
	enum EShaderType
	{
			ST_VertexShader,
			ST_PixelShader,
			ST_GeometryShader,
			ST_ComputeShader,
			ST_HullShader,
			ST_DomainShader,

			ST_NumShaderTypes
	};


	//vs ps gs
	struct SShaderHandle
	{
		void* m_pHandle;
		bool m_bBound;
		EShaderType m_eType;
        SShaderHandle()
		{
			m_bBound = false;
			m_pHandle = NULL;
		 }

	};
	//pass 绑定有 vs ps 等
	struct SShaderPass
	{ 
	  uint32 m_RenderState;          // Render state flags
	  uint8 m_AlphaRef;
	  //shader 
	  SShaderHandlePtr m_VShader;         // Pointer to the vertex shader for the current pass
	  SShaderHandlePtr m_PShader;         // Pointer to fragment shader
	  SShaderHandlePtr m_GShader;         // Pointer to the geometry shader for the current pass

	  int Size()
      {
			int nSize = sizeof(SShaderPass);
			return nSize;
      }
	  SShaderPass& operator = (const SShaderParam sl)
	  {
			memcpy(this, &sl, sizeof(this));
			return *this;
	   }
	};

	struct SShaderTechnique
	{
		  String m_Name;
		  std::vector<SShaderPassPtr> m_Passes;         // General passes
		  int m_Flags;                          
		  int8 m_nTechnique[TTYPE_MAX];          // Next technique in sequence
		  std::vector<RenderTargetPtr> m_RTargets; //渲染目標

	
		int Size()
		{
			//uint32 i;
			int nSize = sizeof(SShaderTechnique);
			//for (i=0; i<m_Passes.size(); i++)
			//{
			//	nSize += m_Passes[i].get().Size();
			//
			//}
			return nSize;
		 }

		  SShaderTechnique():m_Flags(0)
		  {
		
		  }
		  SShaderTechnique& operator = (const SShaderTechnique& sl)
		  {
			  m_Passes = sl.m_Passes;
			  return *this;
		  }
	  //只能指针冲突？
	//  void* operator new( size_t Size ) { void *ptr = malloc(Size); memset(ptr, 0, Size); return ptr; }
	//#if !defined(__SPU__)
	//	void* operator new( size_t Size, const std::nothrow_t& nothrow ) { void *ptr = malloc(Size); if (ptr) memset(ptr, 0, Size); return ptr; }
	//#endif 
	//  void operator delete( void *Ptr ) { free(Ptr); }
	  };

	class CShader
	{
	public:
	  virtual const char *GetName() const =0;
	  virtual int GetFlags() = 0;
	  virtual bool SetTechnique(char *cTachName);
	  virtual bool BeginPass(uint nPass);
	  virtual bool EndPass();
	  virtual bool Load(int nFlags, const char *sShaderName) = 0;
	  virtual std::vector<SShaderParam>& GetParams() = 0;
	  virtual int GetTexId () = 0;
	  virtual CTexture *GetBaseTexture(int *nPass, int *nTU) = 0;
	  virtual unsigned int GetUsedTextureTypes(void) = 0;
	  virtual int Size(int Flags) = 0;
	  virtual int GetTechniqueID(int nTechnique, int nRegisteredTechnique) = 0;
	//  virtual EVertexFormat GetVertexFormat(void) = 0;
	  virtual EShaderType GetShaderType() = 0;
	};
}

	//shader 渲染类型 水 影子=
	enum EShaderDrawType
	{
	SHDT_General,
	SHDT_Light,
	SHDT_Shadow,
	SHDT_Terrain,
	eSHDT_Overlay,
	eSHDT_OceanShore,  
	eSHDT_Fur,
	eSHDT_NoDraw,
	eSHDT_CustomDraw,
	eSHDT_Sky
	};

#endif