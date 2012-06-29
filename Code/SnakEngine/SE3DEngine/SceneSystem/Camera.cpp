
// 摄像机 实现文件
// 版本
//////////////////////////////////////////////////////////////////////////////////
#include <SE3DEngine/SceneSystem/Camera.h>
#include <SE3DEngine/Input/Input.h>
#include <SEMath/Transform.h>
#include <SE3DEngine/Config/Platform.h>
#ifdef MSVS_COMPILER
#pragma warning(push)
#pragma warning(disable: 4305)
#endif


//构造函数
/////////////////
namespace SE
{
	Camera::Camera()
		:m_eControlType(CT_AIRCRAFT),
		m_fMoveSpeed(0.2),
		m_fRotateSpeed (0.2f),
		m_fCameraHeading(0.0f),
		m_fCameraPitch(0.0f)
	{
		m_vEye.Normalize();
		m_vLookAt.Normalize();
		m_vUp.Normalize();
		m_vCustomized[3].Normalize();
		m_ViewMatrix.Identity();
		m_CameraMatrix.Identity();
		m_RefCameraMatrix.Identity();
	}
	//稀构函数
	//////////////////
	Camera::~Camera()
	{
	}
	//设置透视矩阵
	//////////////////////
	void Camera::SetPerspective(const float fFovy, float fAspect, float fNear, float fFar)
	{
#ifdef _SE_D3D11
		m_ProjMatrix = SEMath::MatrixOrthoRH_DirectX(fFovy, fAspect, fNear, fFar);
#else
		m_ProjMatrix = SEMath::MatrixPerspectiveRH_OpenGL(fFovy, fAspect, fNear, fFar);
#endif
		m_fCameraNear = fNear;
		m_fCameraFar = fFar;
		m_fFov= fFovy;
		m_fAspect = fAspect;
	}
	//摄像机位置矩阵初始化
	///////////////////////////
	void Camera::SetView(Vector3 const& vEye, Vector3 const& vLookAt, Vector3 const& vUp)
	{
		m_vEye =  vEye;
		m_vLookAt = vLookAt;
		m_vUp = vUp;
		m_ViewVec = Vector3Normalize(m_vLookAt- m_vEye);

		m_ViewMatrix = SEMath::MatrixLookAtRH(m_vEye, m_vLookAt, m_vUp);

		m_RefCameraMatrix = m_ViewMatrix.FastInvert();
		m_RefCameraMatrix[3].Set(0.0f,0.0f,0.0f, 0.1f);
	}
	//更新摄像机透视矩阵 
	///////////////////////
	//void  Camera::UpdateViewMatrix()
	//{
	//	m_ViewMatrix = SEMath::MatrixLookAtRH(m_vEye, m_vLookAt, m_vUp);
	//	m_CameraMatrix = m_ViewMatrix;
	//	m_CameraMatrix.FastInvert();
	//}
	//设置摄像机位置
	/////////////////////////////

}
