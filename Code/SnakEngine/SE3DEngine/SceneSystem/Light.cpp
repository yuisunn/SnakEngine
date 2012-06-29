#include <SE3DEngine/SceneSystem/Light.h>

#include <SE3DEngine/Comm/SEContext.h>
#include <SE3DEngine/SceneSystem/Camera.h>

namespace SE
{
	CLightSource::CLightSource(uint16 eType)
		: m_eType(eType), m_iAttrib(0), m_bEnabled(true),
			m_vColor(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	CLightSource::~CLightSource()
	{
	}

	uint16 CLightSource::GetType() const
	{
		return m_eType;
	}

	int32 CLightSource::GetAttrib() const
	{
		return m_iAttrib;
	}

	void CLightSource::SetAttrib(int32 iAttrib)
	{
		m_iAttrib = iAttrib;
	}

	bool CLightSource::GetEnabled() const
	{
		return m_bEnabled;
	}

	void CLightSource::SetEnabled(bool bEnabled)
	{
		m_bEnabled = bEnabled;
	}

	//void LightSource::BindUpdateFunc(boost::function<void(LightSource&)> const & update_func)
	//{
	//	update_func_ = update_func;
	//}

	//void LightSource::Update()
	//{
	//	if (update_func_)
	//	{
	//		update_func_(*this);
	//	}
	//}

	void CLightSource::AddToSceneMgr()
	{
		SEContext::Instance().InsSceneMgr().AddLight(this->shared_from_this());
	}

	void CLightSource::DelFromSceneMgr()
	{
		SEContext::Instance().InsSceneMgr().DelLight(this->shared_from_this());
	}

	Vector4 const& CLightSource::GetColor() const
	{
		return m_vColor;
	}

	void CLightSource::SetColor(Vector3 const& cpClr)
	{
		m_vColor = Vector4(cpClr.x, cpClr.y, cpClr.z,
			Vector3Dot(cpClr, Vector3(0.2126f, 0.7152f, 0.0722f)));
	}

	Vector3 const & CLightSource::GetPos() const
	{
		return Vector3::GetZero();
	}

	void CLightSource::SetPos(Vector3 const & /*pos*/)
	{
	}

	Vector3 CLightSource::GetDirection() const
	{
		return Vector3::GetZero();
	}
	
	void CLightSource::SetDirection(Vector3 const& /*dir*/)
	{
	}

	Quaternion const& CLightSource::GetRotation() const
	{
		return Quaternion.Identity();
	}

	void CLightSource::SetRotation(Quaternion const& /*quat*/)
	{
	}

	void CLightSource::ModelMatrix(Matrix4x4 const& /*model*/)
	{
	}

	Vector3 const& CLightSource::Falloff() const
	{
		return Vector3.GetZero();
	}

	void CLightSource::Falloff(Vector3 const& /*fall_off*/)
	{
	}

	float CLightSource::CosInnerAngle() const
	{
		return 0;
	}

	void CLightSource::InnerAngle(float /*angle*/)
	{
	}

	float CLightSource::CosOuterAngle() const
	{
		return 0;
	}

	Vector4 const& CLightSource::CosOuterInner() const
	{
		return Vector4.GetZero();
	}

	void CLightSource::OuterAngle(float /*angle*/)
	{
	}

	CTexturePtr const & CLightSource::ProjectiveTexture() const
	{
		static const CTexturePtr ret;
		return ret;
	}

	void CLightSource::ProjectiveTexture(CTexturePtr const& /*tex*/)
	{
	}

	ConditionalRenderPtr const& CLightSource::ConditionalRenderQuery(uint32 /*index*/) const
	{
		static const ConditionalRenderPtr ret;
		return ret;
	}

	CameraPtr const & CLightSource::SMCamera(uint32_t /*index*/) const
	{
		static const CameraPtr ret;
		return ret;
	}


	AmbientLightSource::AmbientLightSource()
		: LightSource(LT_Ambient)
	{
		attrib_ = LSA_NoShadow;
		color_ = float4(0, 0, 0, 0);
	}

	AmbientLightSource::~AmbientLightSource()
	{
	}


	PointLightSource::PointLightSource()
		: LightSource(LT_Point),
			quat_(Quaternion::Identity()),
			pos_(float3::Zero())
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();
		for (int i = 0; i < 7; ++ i)
		{
			crs_[i] = checked_pointer_cast<ConditionalRender>(rf.MakeConditionalRender());
		}

		for (int i = 0; i < 6; ++ i)
		{
			sm_cameras_[i] = MakeSharedPtr<Camera>();
		}
	}

	PointLightSource::~PointLightSource()
	{
	}

	float3 const & PointLightSource::Position() const
	{
		return pos_;
	}

	void PointLightSource::Position(float3 const & pos)
	{
		pos_ = pos;
		this->UpdateCameras();
	}

	float3 PointLightSource::Direction() const
	{
		return MathLib::transform_quat(float3(0, 0, 1), quat_);
	}
	
	void PointLightSource::Direction(float3 const & dir)
	{
		quat_ = MathLib::unit_axis_to_unit_axis(float3(0, 0, 1), dir);
		this->UpdateCameras();
	}

	Quaternion const & PointLightSource::Rotation() const
	{
		return quat_;
	}

	void PointLightSource::Rotation(Quaternion const & quat)
	{
		quat_ = quat;
		this->UpdateCameras();
	}
	
	void PointLightSource::ModelMatrix(float4x4 const & model)
	{
		float3 scale;
		MathLib::decompose(scale, quat_, pos_, model);
		this->UpdateCameras();
	}

	void PointLightSource::UpdateCameras()
	{
		for (int j = 0; j < 6; ++ j)
		{
			std::pair<float3, float3> ad = CubeMapViewVector<float>(static_cast<Texture::CubeFaces>(j));
			float3 const & d = ad.first;
			float3 const & u = ad.second;

			float3 lookat = MathLib::transform_quat(d, quat_);
			float3 up = MathLib::transform_quat(u, quat_);

			sm_cameras_[j]->ViewParams(pos_, pos_ + lookat, up);
			sm_cameras_[j]->ProjParams(PI / 2, 1, 0.1f, 100.0f);
		}
	}

	float3 const & PointLightSource::Falloff() const
	{
		return falloff_;
	}

	void PointLightSource::Falloff(float3 const & falloff)
	{
		falloff_ = falloff;
	}

	TexturePtr const & PointLightSource::ProjectiveTexture() const
	{
		return projective_tex_;
	}

	void PointLightSource::ProjectiveTexture(TexturePtr const & tex)
	{
		projective_tex_ = tex;
	}

	ConditionalRenderPtr const & PointLightSource::ConditionalRenderQuery(uint32_t index) const
	{
		BOOST_ASSERT(index < crs_.size());
		return crs_[index];
	}

	CameraPtr const & PointLightSource::SMCamera(uint32_t index) const
	{
		BOOST_ASSERT(index < sm_cameras_.size());
		return sm_cameras_[index];
	}


	SpotLightSource::SpotLightSource()
		: LightSource(LT_Spot),
			quat_(Quaternion::Identity()),
			pos_(float3::Zero()),
			sm_camera_(MakeSharedPtr<Camera>())
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();
		cr_ = checked_pointer_cast<ConditionalRender>(rf.MakeConditionalRender());
	}

	SpotLightSource::~SpotLightSource()
	{
	}

	float3 const & SpotLightSource::Position() const
	{
		return pos_;
	}

	void SpotLightSource::Position(float3 const & pos)
	{
		pos_ = pos;
		this->UpdateCamera();
	}

	float3 SpotLightSource::Direction() const
	{
		return MathLib::transform_quat(float3(0, 0, 1), quat_);
	}
	
	void SpotLightSource::Direction(float3 const & dir)
	{
		quat_ = MathLib::unit_axis_to_unit_axis(float3(0, 0, 1), dir);
		this->UpdateCamera();
	}

	Quaternion const & SpotLightSource::Rotation() const
	{
		return quat_;
	}

	void SpotLightSource::Rotation(Quaternion const & quat)
	{
		quat_ = quat;
		this->UpdateCamera();
	}

	void SpotLightSource::ModelMatrix(float4x4 const & model)
	{
		float3 scale;
		MathLib::decompose(scale, quat_, pos_, model);
		this->UpdateCamera();
	}

	void SpotLightSource::UpdateCamera()
	{
		float3 lookat = MathLib::transform_quat(float3(0, 0, 1), quat_);
		float3 up = MathLib::transform_quat(float3(0, 1, 0), quat_);

		sm_camera_->ViewParams(pos_, pos_ + lookat, up);
		sm_camera_->ProjParams(cos_outer_inner_.z(), 1, 0.1f, 100.0f);
	}

	float3 const & SpotLightSource::Falloff() const
	{
		return falloff_;
	}

	void SpotLightSource::Falloff(float3 const & falloff)
	{
		falloff_ = falloff;
	}

	float SpotLightSource::CosInnerAngle() const
	{
		return cos_outer_inner_.y();
	}

	void SpotLightSource::InnerAngle(float angle)
	{
		cos_outer_inner_.y() = cos(angle);
	}

	float SpotLightSource::CosOuterAngle() const
	{
		return cos_outer_inner_.x();
	}

	void SpotLightSource::OuterAngle(float angle)
	{
		cos_outer_inner_.x() = cos(angle);
		cos_outer_inner_.z() = angle * 2;
		cos_outer_inner_.w() = tan(angle);

		this->UpdateCamera();
	}

	float4 const & SpotLightSource::CosOuterInner() const
	{
		return cos_outer_inner_;
	}

	TexturePtr const & SpotLightSource::ProjectiveTexture() const
	{
		return projective_tex_;
	}

	void SpotLightSource::ProjectiveTexture(TexturePtr const & tex)
	{
		projective_tex_ = tex;
	}

	ConditionalRenderPtr const & SpotLightSource::ConditionalRenderQuery(uint32_t /*index*/) const
	{
		return cr_;
	}

	CameraPtr const & SpotLightSource::SMCamera(uint32_t /*index*/) const
	{
		return sm_camera_;
	}


	DirectionalLightSource::DirectionalLightSource()
		: LightSource(LT_Directional),
			quat_(Quaternion::Identity())
	{
		attrib_ = LSA_NoShadow;
	}

	DirectionalLightSource::~DirectionalLightSource()
	{
	}

	Quaternion const & DirectionalLightSource::Rotation() const
	{
		return quat_;
	}

	float3 DirectionalLightSource::Direction() const
	{
		return MathLib::transform_quat(float3(0, 0, 1), quat_);
	}
	
	void DirectionalLightSource::Direction(float3 const & dir)
	{
		quat_ = MathLib::unit_axis_to_unit_axis(float3(0, 0, 1), MathLib::normalize(dir));
	}

	void DirectionalLightSource::Rotation(Quaternion const & quat)
	{
		quat_ = quat;
	}

	void DirectionalLightSource::ModelMatrix(float4x4 const & model)
	{
		float3 scale, pos;
		MathLib::decompose(scale, quat_, pos, model);
	}

	float3 const & DirectionalLightSource::Falloff() const
	{
		return falloff_;
	}

	void DirectionalLightSource::Falloff(float3 const & falloff)
	{
		falloff_ = falloff;
	}
}
