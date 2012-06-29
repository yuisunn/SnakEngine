// ƽ���� ͷ�ļ�
// �汾
///////////////////////////
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_


#include <SEMath/Matrix4x4.h>
#include <SEMath/Vector4.h>
#include <SEMath/BoundingGeo.h>

namespace SE
{
	class Frustum
	{
	public:
		Frustum(void);
		~Frustum(void);
		enum VIS
		{
				VIS_YES,
				VIS_NO,
				VIS_PART,
		};

		void Construct(Matrix4x4  &clip); //����ÿ�����λ��
		int  VisiblePoint(const Vector4& v ) ;
		Frustum::VIS VisibleAABB(const AABB& box);
		//bool CheckRectangle( D3DXVector4 centerPos, float size );
		//bool CheckSphere( D3DXVector4 centerPos, float radius );
	protected:
		Vector4 m_planes[6]; //6��ƽ����Ϣ
		int m_flag[6];

	};
}

#endif
