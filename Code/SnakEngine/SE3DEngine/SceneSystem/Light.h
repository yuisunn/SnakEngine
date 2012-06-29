#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <SE3DEngine/Comm/Def.h>
#include <SEMath/Matrix4x4.h>
#include <SEMath/Vector3.h>
#include <SEMath/Vector4.h>
#include <SEMath/Matrix4x4.h>
#include <SEMath/Quaternion.h>


namespace SE
{
	enum LightType
	{
		LT_Ambient = 0,
		LT_Point,
		LT_Directional,
		LT_Spot,

		LT_UnKnow
	};

	enum LightSrcAttrib
	{
		LSA_NoShadow = 1UL << 0,
		LSA_NoDiffuse = 1UL << 1,
		LSA_NoSpecular = 1UL << 2,
		LSA_IndirectLighting = 1UL << 3
	};
	//public boost::enable_shared_from_this<LightSource>
	class _DLL_API CLightSource
	{
	public:
		explicit CLightSource(uint16 eType);
		virtual ~CLightSource();
		bool CLightSource::GetEnabled() const;
		void CLightSource::SetEnabled(bool bEnabled);
		uint16 GetType() const;
		 int32 GetAttrib() const;
		 void SetAttrib(int32 iAttrib);
		//void BindUpdateFunc(boost::function<void(LightSource&)> const & update_func);

		virtual void Update();

		virtual void AddToSceneMgr();
		virtual void DelFromSceneMgr();

		Vector4 const & GetColor() const;
		void SetColor(Vector3 const& cpClr);

		virtual Vector3 const& GetPos() const;
		virtual void SetPos(Vector3 const& cpPos);
		virtual Vector3 GetDirection() const;
		virtual void SetDirection(Vector3 const& cpDir);
		virtual Quaternion const& GetRotation() const;
		virtual void SetRotation(Quaternion const& cpQuat);
		virtual void ModelMatrix(Matrix4x4 const& cpModel);
		virtual Vector3 const & Falloff() const;
		virtual void Falloff(Vector3 const& cpFallOff);
		virtual float CosInnerAngle() const;
		virtual void InnerAngle(float fAngle);
		virtual float CosOuterAngle() const;
		virtual void OuterAngle(float fAngle);
		virtual Vector4 const & CosOuterInner() const;
           


		virtual CTexturePtr const& ProjectiveTexture() const;
		virtual void ProjectiveTexture(CTexturePtr const& cpTex);

		virtual ConditionalRenderPtr const& ConditionalRenderQuery(uint32 iIndex) const;
		virtual CameraPtr const & SMCamera(uint32 iIndex) const; 
	protected:
		uint16 m_eType;
		int32 m_iAttrib;
		bool m_bEnabled;
		Vector4 m_vColor;

		//boost::function<void(LightSource&)> update_func_;
	};

}


#endif