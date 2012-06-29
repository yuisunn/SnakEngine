#ifndef Tran_H
#define Tran_H

#include <SEMath/Matrix4x4.h>
#include <SEMath/Vector3.h>
namespace SEMath
{
	//// left hand coord system
	Matrix4x4 MatrixLookAtLH(Vector3 &eye, Vector3 &lookat, Vector3 &up)
	{
		Vector3 up_normalized = Vector3Normalize(up);
		Vector3 zaxis = (lookat - eye); zaxis.Normalize();
		Vector3 xaxis = Vector3Cross(up_normalized, zaxis);
		Vector3 yaxis = Vector3Cross(zaxis, xaxis);

		Matrix4x4 matrix; 
		matrix.Identity();

		matrix.SetColumn(0, xaxis, 0);
		matrix.SetColumn(1, yaxis, 0);
		matrix.SetColumn(2, zaxis, 0);
		matrix[3][0] = -Vector3Dot(xaxis, eye);
		matrix[3][1] = -Vector3Dot(yaxis, eye);
		matrix[3][2] = -Vector3Dot(zaxis, eye);

		return matrix;
	}
	//
	//// right hand coord system
	//// eye = `描繷竚`
	//// lookat = `描繷癸非竚`
	//// up = `描繷タよよ`
	Matrix4x4 MatrixLookAtRH(Vector3 &eye, Vector3 &lookat, Vector3 &up)
	{
		Vector3 up_normalized = Vector3Normalize(up);
		Vector3 zaxis = (eye - lookat); zaxis.Normalize();
		Vector3 xaxis = Vector3Cross(up_normalized, zaxis);
		Vector3 yaxis = Vector3Cross(zaxis, xaxis);

		Matrix4x4 matrix; 
		matrix.Identity();

		matrix.SetColumn(0, xaxis, 0);
		matrix.SetColumn(1, yaxis, 0);
		matrix.SetColumn(2, zaxis, 0);
		matrix[3][0] = -Vector3Dot(xaxis, eye);
		matrix[3][1] = -Vector3Dot(yaxis, eye);
		matrix[3][2] = -Vector3Dot(zaxis, eye);

		return matrix;
	}



	Matrix4x4 MatrixPerspectiveLH_DirectX(float fovy, float aspect, float z_near, float z_far)
	{
		Matrix4x4 matrix;
		matrix.Identity();

		float fovy_radian = SEMath::DegreeToRadian(fovy);
		float yscale = SEMath::Cot(fovy_radian/2.0f);
		float xscale = yscale * aspect;

		matrix[0][0] = xscale;
		matrix[1][1] = yscale;
		matrix[2][2] = z_far / (z_far - z_near);
		matrix[2][3] = 1.0f;
		matrix[3][2] = -(z_near * z_far) /(z_far - z_near);
		matrix[3][3] = 0.0f;

		return matrix;
	}

	Matrix4x4 MatrixPerspectiveRH_OpenGL(float fovy, float aspect, float z_near, float z_far)
	{
		Matrix4x4 matrix;
		matrix.Identity();

		float fovy_radian = SEMath::DegreeToRadian(fovy);
		float yscale =  SEMath::Cot(fovy_radian/2.0f);
		float xscale = yscale * aspect;

		matrix[0][0] = xscale;
		matrix[1][1] = yscale;
		matrix[2][2] = (z_far + z_near)/(z_near - z_far);
		matrix[2][3] = -1.0f;
		matrix[3][2] = 2.0f * z_far * z_near / (z_near - z_far);
		matrix[3][3] = 0.0f;
		return matrix;
	}

	//垂直方向看到的距离 w  ...h
	Matrix4x4 MatrixOrthoRH_DirectX(float w, float h, float z_near, float z_far)
	{
		Matrix4x4 matrix;
		matrix.Identity();
		
		matrix[0][0] = 2.0f/w;
		matrix[1][1] = 2.0f/h;
		matrix[2][2] = 1.0f/(z_near - z_far);
		matrix[3][2] = z_near / (z_near - z_far);

		return matrix;
	}

}
#endif