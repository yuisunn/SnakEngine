#ifndef _IMesh_H_
#define _IMesh_H_

#include <SE3DEngine/RenderSystem/CGraphBuffer.h>
#include <SE3DEngine/RenderSystem/IMaterial.h>
#include <SEMath/Vector4.h>
#include <SEMath/Quaternion.h>
#include <SE3DEngine/Comm/XmlDom.h>
#include <SEMath/BoundingGeo.h>

namespace SE
{
	enum ERenderMeshType
	{
	  RMT_Static = 0,
 
	};
	
	//骨骼動畫用的
	struct  SKeyFrame
	{
		std::vector<uint32> frame_id;
		std::vector<Quaternion> bind_real;
		std::vector<Quaternion> bind_dual;

		std::pair<Quaternion, Quaternion> Frame(float frame) const;
	};
	struct SJoint
	{
		std::string name;

		Quaternion bind_real;
		Quaternion bind_dual;

		Quaternion inverse_origin_real;
		Quaternion inverse_origin_dual;

		int16 parent;
	};
	struct SMeshSubSet
	{
		Vector4 vCenter;
		float fRadius;

		int nFirstIndexId;
		int nNumIndices;

		int nFirstVertId;
		int nNumVerts;

		int nMatID; // Material Sub-object id.
		int nMatFlags; // Special Material flags.
		int nPhysicalizeType; // Type of physicalization for this subset.
		//全局sub array
	};
	typedef std::vector<std::pair<std::string, std::string> > TextureSlotsType;
    struct  STexMaterial
	{
		Vector4 vAmbient;
		Vector4 vDiffuse;
		Vector4 vSpecular;
		Vector4 vEmit;
		float fOpacity;
		float fSpecular_level;
		float fShininess;

		TextureSlotsType sTextureslots;
	};
 

	//渲染用的mesh
	struct IRenderMesh
    {
	  // Returns type name given to the render mesh on creation time.
	  virtual const char* GetTypeName() = 0;
	  // Returns the name of the source given to the render mesh on creation time.
	  virtual const char* GetSourceName() const = 0;

	  virtual int  GetIndicesCount() = 0;
	  virtual int  GetVerticesCount() = 0;
	  virtual EVertexFormat GetVertexFormat() = 0;
	  virtual ERenderMeshType GetMeshType() = 0;



	  virtual IRenderMesh *GenerateMorphWeights() = 0; 
	  virtual IRenderMesh *GetMorphBuddy() = 0;
	  virtual void SetMorphBuddy(IRenderMesh *pMorph) = 0;

	  virtual bool UpdateVertices(const void *pVertBuffer, int nVertCount, int nOffset, int nStream=VSF_GENERAL) = 0;
	  virtual bool UpdateIndices(const uint16 *pNewInds, int nInds, int nOffsInd=0) = 0;
	  virtual IMaterial* GetMaterial() = 0;
	  virtual void SetMaterial( IMaterial * pNewMat, int nCustomTID=0) = 0;
	  virtual IRenderMesh *GetVertexContainer() = 0;
	  virtual void SetVertexContainer(IRenderMesh *pBuf) = 0;

	  virtual void SetBBox(const Vector4& vBoxMin, const Vector4& vBoxMax) = 0;
	  virtual void GetBBox(Vector4& vBoxMin, Vector4& vBoxMax) = 0;
	  virtual void UpdateBBoxFromMesh() = 0;
	  virtual uint32 * GetPhysVertexMap() = 0;
	  virtual void SetPhysVertexMap(uint32 * pVtxMap) = 0;
	  virtual bool IsEmpty() = 0;

	  virtual byte *GetPosPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;
	  virtual byte *GetColorPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;
	  virtual byte *GetNormPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;
	  virtual byte *GetUVPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;

	  virtual byte *GetTangentPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;
	  virtual byte *GetBinormalPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;

	  virtual byte *GetHWSkinPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;
	  virtual byte *GetShapePtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;
	  virtual byte *GetMorphTargetPtr(int32& nStride, uint32 nFlags, int32 nOffset=0) = 0;

	  virtual void UnlockStream(int nStream) = 0;
	  virtual void UnlockIndexStream() = 0;

	  virtual uint16 *GetIndexPtr(uint32 nFlags, int32 nOffset=0) = 0;


	  // Debug draw this render mesh.
	  virtual void DebugDraw(const struct SGeometryDebugDrawInfo &info, uint32 nVisibleChunksMask=~0) = 0;

	  // Returns mesh memory usage and add it to the CrySizer (if not NULL).
	  // Arguments:
	  //     pSizer - Sizer interface, can be NULL if caller only want to calculate size
	  //     nType - see EMemoryUsageArgument

	  // Get allocated only in video memory or only in system memory.
	  virtual int GetAllocatedBytes( bool bVideoMem ) = 0;
	  virtual float GetAverageTrisNumPerChunk(IMaterial * pMat) = 0;
	  virtual void KeepSysMesh(bool keep) = 0;	// workaround: temp workaround for GDC-888
		virtual void UnKeepSysMesh() = 0;
	  virtual void SetMeshLod( int nLod ) = 0;
   };

}

#endif

