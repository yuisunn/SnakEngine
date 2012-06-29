#ifndef _MeshModel_H_
#define _MeshModel_H_

#include <SE3DEngine/Mesh/IMesh.h>
#include <SE3DEngine/RenderSystem/GraphBuffer.h>
#include <vector>
#include <SE3DEngine/Comm/Def.h>

namespace SE
{

		//静态 代表1用同一个textrue同一个material（d3d的）相当于sub
	class StaticMesh //: public IRenderMesh
	{
	public:
		//void AddVertexStream(void const * buf, uint32 size, EVertexElementType const & ve, uint32 access_hint);
		//void AddIndexStream(void const * buf, uint32 size, ElementFormat format, uint32 access_hint);
		void UpdateVerteices(const void *pVertBuffer, int nVertCount, int nOffset);
		void LockVB(int nStream, uint32 nFlags, int nOffset, int nVerts, int *nStride);
		void LockIB();
		void UnlockVertexStream();
		void UnlockIndexStream();
		//void SetAABB(AABB const & box);
		//AABB& GetAABB() const;
	protected:
 
		IRenderPtr m_pIR;
	};
	class RenderMesh : public IRenderMesh
	{


	}

	class SkinnedModel

}
#endif

