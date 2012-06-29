// 摄像机 头文件
// 版本
//////////////////////////////////////////////////////////////////////////////////

#ifndef _Camera_H_
#define _Camera_H_

#include <SEMath/Vector3.h>
#include <SEMath/Matrix4x4.h>
#include <SE3DEngine/SceneSystem/Frustum.h>
#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	enum ECameraType
	{
		CT_AIRCRAFT,
		CT_Normal

	};
	
	class Camera
	{
	public:
		Camera();
		~Camera();	

		void RotateObject(float fTimeAdvance);
		void SetObjectPosition(Vector3 &vPosition);

		void SetView(const Vector3 &vEye, float fHeading, float fPitch);
		void SetView(Vector3 const& vEye, Vector3 const& vLookAt, Vector3 const& vUp);
		void SetPerspective(const float fovy, const float aspect, const float z_near, const float z_far);

		void update();

		void SetRotateSpeed(const float speed) { m_fRotateSpeed = speed; }
		void SetMoveSpeed(const float speed) { m_fMoveSpeed = speed; }

		Matrix4x4 const& GetProjection (void) const { return m_ProjMatrix; }
		Matrix4x4 const& GetViewMatrix(void) const { return m_ViewMatrix; }
		Matrix4x4 const& GetCameraMatrix(void) const { return m_CameraMatrix; }

		float const& GetCameraFar(void) const { return m_fCameraFar; }
		float const& GetCameraNear(void) const { return m_fCameraNear; }
		Vector3 const& GetEye() const { return m_vEye; }
		Vector3 const& GetLookAt() const { return m_vLookAt; }
		Vector3 const & GetUpVec() const { return m_vUp; }
		float GetFov() const { return m_fFov;}
		float GetAspect() const { return m_fAspect;}
		uint16 GetCameraType() const { return m_eControlType;}


	protected:
		String m_Name;
		Matrix4x4 m_ProjMatrix;
		//Matrix4x4 m_WorldMatrix;
		Matrix4x4 m_ViewMatrix;

		Matrix4x4 m_CameraMatrix;// view矩陣的逆
		Matrix4x4 m_RefCameraMatrix;//view矩陣的逆去掉旋轉
	
		Vector3 m_vEye, m_vLookAt, m_vUp;
		Vector3 m_ViewVec;
		Vector3 m_vCustomized[3];

		//摄像机移动速度
		float m_fRotateSpeed;
		float m_fCameraNear;
		float m_fCameraFar;
		float m_fFov;
		float m_fAspect;
		float m_fMoveSpeed;
		float m_fCameraHeading;
		float m_fCameraPitch;
		//
		Frustum* m_pFrustum;
		uint16 m_eControlType;

	};
}

#endif