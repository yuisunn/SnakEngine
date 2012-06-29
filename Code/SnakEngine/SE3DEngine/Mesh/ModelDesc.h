#ifndef _ModelDesc_H_
#define _ModelDesc_H_

#include <SE3DEngine/Mesh/IMesh.h>
#include <SE3DEngine/RenderSystem/GraphBuffer.h>
#include <vector>
#include <SE3DEngine/ResourceSystem/SEResourceManager.h>
#include <SE3DEngine/Comm/Def.h>
 


namespace SE
{

	class ModelDesc : public IResource
	{
	public:
		ModelDesc()
		{

		}
		struct SModelInfo
		{
			uint32  uMaccessHint;
			std::vector<STexMaterial> texMtls;
			std::vector<vertex_element> vMergedVes;
			char cAll_is_index_16_bit;
			std::vector<std::vector<uint8> > uMergedBuff;
			std::vector<uint8> uMergedIndices;
			std::vector<std::string> sMeshNames;
			std::vector<int32> iMtlIDs;
			std::vector<AABB> boundBox;
			std::vector<uint32> uMeshNumVertices;
			std::vector<uint32> uMeshBaseVertices;
			std::vector<uint32> uMeshNumIndices;
			std::vector<uint32> uMeshStartIndices;
			std::vector<SJoint> joints;
			std::vector<SKeyFrame> keyFrames;
			int32 iStartFrame;
			int32 iEndFrame;
			int32 iFrameRate;

 
		};
	public:
	
		void LoadModelXml(String const &sName, SModelInfo model)
		{
			DataStreamXmlPtr data;
			ModelDescPtr res = ResManager::Instance().Create(sName);


		}
		void SyncLoadModel();
		void SaveModel();
	private:
		SModelInfo m_model;
	//‰÷»æ
	};

}

#endif

