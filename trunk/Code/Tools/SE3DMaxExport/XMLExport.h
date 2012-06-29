#ifndef _SE3DXMLExport_H_
#define _SE3DXMLExport_H_

#include "max.h"
#include <vector>
#include "DateDefine.h"


class IPhysiqueExport;
class ISkin;


class SE3DXMLExport
{
public:
	SE3DXMLExport(INode* root_node, int joints_per_ver, int cur_time, int start_frame, int end_frame, bool combine_meshes);

	void ExportObjects(std::vector<INode*> const & nodes);
	void WriteXml(std::string const & file_name, SExportAttrs const & eva);

private:
	void ExtractObject(INode* node);
	void ExtractBone(INode* node);
	void RemoveRedundantJoints();
	void RemoveRedundantMtls();
	void CombineMeshesWithSameMtl();
	void SortMeshesByMtl();

	Point3 PointFromMatrix(Matrix3 const & mat);
	Quat QuatFromMatrix(Matrix3 const & mat);

	void FindJoints(INode* node);
	void ExtractAllJoint();
	void AddJointWeight(binds_t& binds, std::string const & joint_name, float weight);

	void PhysiqueModifier(Modifier* mod, INode* node,
		std::vector<std::pair<Point3, binds_t> >& positions);
	void SkinModifier(Modifier* mod, INode* node,
		std::vector<std::pair<Point3, binds_t> >& positions);

	void GetMaterial(materials_t& mtls, std::vector<std::map<int, std::pair<Matrix3, int> > >& uv_transss, Mtl* max_mtl);



	typedef std::vector<SObjectInfo> objects_info_t;
	objects_info_t m_ObjsInfo;
	materials_t m_ObjsMtl;

	INode* m_pRootNode;

	float m_fScale;

	joints_t m_Joints;
	int m_nJointsPerVer;
	bool m_bCombineMeshes;

	std::map<std::string, SJointMat> joint_nodes_;

	int m_nCurTime;
	int m_nStartFrame;
	int m_nEndFrame;
	int m_nFrameRate;
	key_frames_t m_KeyFrame;

	std::vector<IPhysiqueExport*> m_Physiques;
	std::vector<Modifier*> m_PhysiqueMods;
	std::vector<ISkin*> m_Skins;
	std::vector<Modifier*> m_SkinMods;
};



#endif