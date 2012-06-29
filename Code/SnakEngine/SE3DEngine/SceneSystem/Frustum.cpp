// ƽ���� ʵ���ļ�
// �汾
//////////////////////////////////////////////////////////////////////////////////
#include <SE3DEngine/SceneSystem/Frustum.h>
#include <SEMath/BoundingGeo.h>
#include <SEMath/FastMath.h>


namespace SE
{
	//���캯��
	////////////////////////////
	Frustum::Frustum(void)
	{
		for(int i=0;i<6;i++)
		m_planes[i]=Vector4(0.0f);
	}
	//ϡ���캯��
	////////////////////////////
	Frustum::~Frustum(void)
	{
	}
	//����6���� clip �� wvp matrix
	/////////////////////////
	void Frustum::Construct(Matrix4x4 & clip)
	{
		Vector4  & column1(clip.GetColumn(0));
		Vector4  & column2(clip.GetColumn(1));
		Vector4  & column3(clip.GetColumn(2));
		Vector4  & column4(clip.GetColumn(3));
		//����ƽ�淽��
		//Near��    (P0,P4,P5)     n=(0,0,-1),d=0              0x+0y-1z+0=0     
		m_planes[0] = column4 - column3;//����������ƽ�淽�̣�ʹ|a,b,c| == 1��
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
		//��һ
		for(int i=0; i<6; i++)
		{
			m_planes[i].Normalize();
		}
		//������ǣ�����
		//  build a bit-field that will tell us the indices for the nearest and farthest vertices from each plane...
		for(int i=0; i<6; i++)
		{
			m_flag[i]  = ((m_planes[i].x<0) ? 1 : 0) | ((m_planes[i].y<0) ? 2 : 0) | ((m_planes[i].z) ? 4 : 0);
		}

	}
	//����ɼ�
	///////////////
	int Frustum::VisiblePoint(const Vector4& v ) 
	{
		int clipCode = 0;

		for ( register int i=0, j = 1; i<6; i++, j<<=1 )
			if ( SEMath::dot_coord(m_planes[i],v) <= 0.0f )
				clipCode |= j;
	
		return clipCode;
	}
	// ��������,�������ڲ��ڿɼ�����
	//////////////////////////////////////////////
	Frustum::VIS Frustum::VisibleAABB( const AABB& box ) 
	{
		bool intersect = false;
	
		for(int i=0; i<6; i++)
		{
		int const n = m_flag[i];
		//�����֪����
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

	// ���μ�⣬����������������ǣ���Ĭ�Ͼ����� XOY ƽ��ƽ�У������Ǵ�ֱ��Y�������ϵ�����
	// ֻҪ��һ�������ڲü���֮�ھ���Ϊ�ɼ�
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

	// ������
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