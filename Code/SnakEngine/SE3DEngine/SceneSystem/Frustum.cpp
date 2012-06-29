// 平截体 实现文件
// 版本
//////////////////////////////////////////////////////////////////////////////////
#include <SE3DEngine/SceneSystem/Frustum.h>
#include <SEMath/BoundingGeo.h>
#include <SEMath/FastMath.h>


namespace SE
{
	//构造函數
	////////////////////////////
	Frustum::Frustum(void)
	{
		for(int i=0;i<6;i++)
		m_planes[i]=Vector4(0.0f);
	}
	//稀构造函數
	////////////////////////////
	Frustum::~Frustum(void)
	{
	}
	//生成6个面 clip 是 wvp matrix
	/////////////////////////
	void Frustum::Construct(Matrix4x4 & clip)
	{
		Vector4  & column1(clip.GetColumn(0));
		Vector4  & column2(clip.GetColumn(1));
		Vector4  & column3(clip.GetColumn(2));
		Vector4  & column4(clip.GetColumn(3));
		//根据平面方程
		//Near：    (P0,P4,P5)     n=(0,0,-1),d=0              0x+0y-1z+0=0     
		m_planes[0] = column4 - column3;//这个函数规格化平面方程，使|a,b,c| == 1。
		//Far:      (P2,P6,P7)     n=(0,0,1),d=-1              0x+0y+1z-1=0  
		m_planes[1] = column4 + column3;
		//Left:     (P0,P3,P7)     n=(-1,0,0),d=-1          -1x+0y+0z-1=0     
		m_planes[2] = column4 - column1;
		//Right:    (P1,P5,P6)     n=(1,0,0),d=-1              1x+0y+0z-1=0    
		m_planes[3] = column4 + column1;
		//Top:      (P4,P7,P6)     n=(0,1,0),d=-1              0x+1y+0z-1=0   
		m_planes[4] = column4 + column2;
		//Bottom:   (P0,P1,P2)     n=(0,-1,0),d=-1            0x-1y+0z-1=0     
		m_planes[5] = column4 - column2;
		//归一
		for(int i=0; i<6; i++)
		{
			m_planes[i].Normalize();
		}
		//做个标记？？？
		//  build a bit-field that will tell us the indices for the nearest and farthest vertices from each plane...
		for(int i=0; i<6; i++)
		{
			m_flag[i]  = ((m_planes[i].x<0) ? 1 : 0) | ((m_planes[i].y<0) ? 2 : 0) | ((m_planes[i].z) ? 4 : 0);
		}

	}
	//检查点可见
	///////////////
	int Frustum::VisiblePoint(const Vector4& v ) 
	{
		int clipCode = 0;

		for ( register int i=0, j = 1; i<6; i++, j<<=1 )
			if ( SEMath::dot_coord(m_planes[i],v) <= 0.0f )
				clipCode |= j;
	
		return clipCode;
	}
	// 立方体检测,立方体在不在可见区域
	//////////////////////////////////////////////
	Frustum::VIS Frustum::VisibleAABB( const AABB& box ) 
	{
		bool intersect = false;
	
		for(int i=0; i<6; i++)
		{
		int const n = m_flag[i];
		//这个不知所以
		// v1 is diagonally opposed to v0
		Vector4 v0((n & 1) ? box.min.x : box.max.x, (n & 2) ? box.min.y : box.max.y, (n & 4) ? box.min.z : box.max.z);
		Vector4 v1((n & 1) ? box.max.x : box.min.x, (n & 2) ? box.max.y : box.min.y, (n & 4) ? box.max.z : box.min.z);

		if (SEMath::dot_coord(m_planes[i], v0) < 0)
		{
				return VIS_NO;
		}
		if (SEMath::dot_coord(m_planes[i], v1) < 0)
		{
				intersect = true;
		}
		}
		return intersect ? VIS_PART : VIS_YES;
	}

	// 矩形检测，与立方体检测的区别是，它默认矩形与 XOY 平面平行，不考虑垂直（Y）方向上的因素
	// 只要有一个顶点在裁减体之内就认为可见
	//////////////////////////////////////////////////
	//bool Frustum::CheckRectangle( D3DXVector4 centerPos, float size )
	//{
	//	float sizeDivd2 = size/2;
	//	D3DXVector4 posVertices[4];
	//	posVertices[0] = centerPos + D3DXVector4(-sizeDivd2, 0,-sizeDivd2 );
	//	posVertices[1] = centerPos + D3DXVector4(-sizeDivd2, 0, sizeDivd2 );
	//	posVertices[2] = centerPos + D3DXVector4( sizeDivd2, 0, sizeDivd2 );
	//	posVertices[3] = centerPos + D3DXVector4( sizeDivd2, 0,-sizeDivd2 );
	//	for(int i=0;i<4;i++)
	//	{
	//		if( CheckPoint( posVertices[i].x, posVertices[i].y, posVertices[i].z ))
	//			return TRUE;
	//	}
	//	return FALSE;
	//}

	// 球体检测
	///////////////////////////////////////////////////////////
	//bool Frustum::CheckSphere( D3DXVector4 centerPos, float radius )
	//{
	//	for(int i=0;i<6;i++)
	//	{
	//		if( D3DXPlaneDotCoord(&m_planes[i], &centerPos) < -radius )
	//			return FALSE;
	//	}
	//	return TRUE;
	//}

}