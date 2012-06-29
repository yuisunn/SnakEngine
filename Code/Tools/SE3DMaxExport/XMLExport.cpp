#include <max.h>
#include <modstack.h>
#include <stdmat.h>
#include <iparamb2.h>
#if VERSION_3DSMAX >= 7 << 16
#include <CS/phyexp.h>
#else
#include <phyexp.h>
#endif
#include <iskin.h>


#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>
#include <limits>
#include <functional>

#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include "XMLUtility.h"
#include "XMLExport.h"


//操作符
bool operator<(SVertexIndex const & lhs, SVertexIndex const & rhs)
{
	if (lhs.pos_index < rhs.pos_index)
	{
		return true;
	}
	else
	{
		if (lhs.pos_index > rhs.pos_index)
		{
			return false;
		}
		else
		{
			if (lhs.tex_indices < rhs.tex_indices)
			{
				return true;
			}
			else
			{
				if (lhs.tex_indices > rhs.tex_indices)
				{
					return false;
				}
				else
				{
					if (lhs.sm_index < rhs.sm_index)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
}

struct less_Point2 : public std::binary_function<Point2, Point2, bool>
{
	bool operator()(Point2 const & lhs, Point2 const & rhs) const
	{
		if (lhs.x < rhs.x)
		{
			return true;
		}
		else
		{
			if (lhs.x > rhs.x)
			{
				return false;
			}
			else
			{
				if (lhs.y < rhs.y)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
};

bool BindCmp(std::pair<std::string, float> const& lhs,
	std::pair<std::string, float> const& rhs)
{
	return lhs.second > rhs.second;
}

Point3 ComputeNormal(Point3 const & v0XYZ, Point3 const & v1XYZ, Point3 const & v2XYZ)
{
	Point3 v1v0 = v1XYZ - v0XYZ;
	Point3 v2v0 = v2XYZ - v0XYZ;

	return CrossProd(v1v0, v2v0);
}

void ComputeTangent(Point3& tangent, Point3& binormal, Point3 const & v0XYZ, Point3 const & v1XYZ, Point3 const & v2XYZ,
	Point2 const & v0Tex, Point2 const & v1Tex, Point2 const & v2Tex,
	Point3 const & normal)
{
	Point3 v1v0 = v1XYZ - v0XYZ;
	Point3 v2v0 = v2XYZ - v0XYZ;

	float s1 = v1Tex.x - v0Tex.x;
	float t1 = v1Tex.y - v0Tex.y;

	float s2 = v2Tex.x - v0Tex.x;
	float t2 = v2Tex.y - v0Tex.y;

	float denominator = s1 * t2 - s2 * t1;
	if (abs(denominator) < std::numeric_limits<float>::epsilon())
	{
		tangent = Point3(1, 0, 0);
		binormal = Point3(0, 1, 0);
	}
	else
	{
		tangent = (t2 * v1v0 - t1 * v2v0) / denominator;
		binormal = (s1 * v2v0 - s2 * v1v0) / denominator;
	}
}

Quat QuatTransToUDQ(Quat const & q, Point3 const & t)
{
	return Quat(+0.5f * (+t[0] * q[3] + t[1] * q[2] - t[2] * q[1]),
		+0.5f * (-t[0] * q[2] + t[1] * q[3] + t[2] * q[0]),
		+0.5f * (+t[0] * q[1] - t[1] * q[0] + t[2] * q[3]),
		-0.5f * (+t[0] * q[0] + t[1] * q[1] + t[2] * q[2]));
}



SE3DXMLExport::SE3DXMLExport(INode* root_node, int joints_per_ver, int cur_time, int start_frame, int end_frame, bool combine_meshes)
					: m_pRootNode(root_node),
					  m_fScale(static_cast<float>(GetMasterScale(UNITS_METERS))),
					  m_nJointsPerVer(joints_per_ver),
						m_nCurTime(cur_time),
						m_nStartFrame(start_frame), m_nEndFrame(end_frame),
						m_nFrameRate(GetFrameRate()),
						m_bCombineMeshes(combine_meshes)
{
}
//找joint
void SE3DXMLExport::FindJoints(INode* node)
{
	if (IsBone(node))
	{
		std::string joint_name = tstr_to_str(node->GetName());
		SJointMat jam;
		jam.joint_node = node;
		joint_nodes_[joint_name] = jam;
	}
	for (int i = 0; i < node->NumberOfChildren(); ++ i)
	{
		this->FindJoints(node->GetChildNode(i));
	}
}
//导出对象 包括bone
void SE3DXMLExport::ExportObjects(std::vector<INode*> const & nodes)
{
	//骨骼动画 帧动画？
	if (m_nJointsPerVer > 0)
	{
		// root bone
		SJoint root;
		//初始化
		root.real.Identity();
		root.dual.x = root.dual.y = root.dual.z = root.dual.w = 0;
		root.parent_name = "";
		std::string root_name = tstr_to_str(m_pRootNode->GetName());
		m_Joints.insert(std::make_pair(root_name, root));

		int tpf = GetTicksPerFrame();
		//帧动画 变换4q导出
		SKeyframe kf;
		kf.joint = root_name;
		for (int i = m_nStartFrame; i < m_nEndFrame; ++ i)
		{
			Matrix3 root_tm = m_pRootNode->GetNodeTM(i * tpf);

			Quat quat = this->QuatFromMatrix(root_tm);
			Point3 pos = this->PointFromMatrix(root_tm);
			kf.reals.push_back(quat);
			kf.duals.push_back(QuatTransToUDQ(quat, pos));
		}
		m_KeyFrame.push_back(kf);

		this->FindJoints(m_pRootNode);

		//导出modifier
		m_Physiques.clear();
		m_PhysiqueMods.clear();
		m_Skins.clear();
		m_SkinMods.clear();
		BOOST_FOREACH(BOOST_TYPEOF(nodes)::const_reference node, nodes)
		{
			Object* obj_ref = node->GetObjectRef();
			while ((obj_ref != NULL) && (GEN_DERIVOB_CLASS_ID == obj_ref->SuperClassID()))
			{
				IDerivedObject* derived_obj = static_cast<IDerivedObject*>(obj_ref);
				
				// Iterate over all entries of the modifier stack.
				for (int mod_stack_index = 0; mod_stack_index < derived_obj->NumModifiers(); ++ mod_stack_index)
				{
					Modifier* mod = derived_obj->GetModifier(mod_stack_index);
					if (Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B) == mod->ClassID())
					{
						IPhysiqueExport* phy_exp = static_cast<IPhysiqueExport*>(mod->GetInterface(I_PHYINTERFACE));
						if (phy_exp != NULL)
						{
							m_Physiques.push_back(phy_exp);
							m_PhysiqueMods.push_back(mod);
						}
					}
					else
					{
						if (SKIN_CLASSID == mod->ClassID())
						{
							ISkin* skin = static_cast<ISkin*>(mod->GetInterface(I_SKIN));
							if (skin != NULL)
							{
								m_Skins.push_back(skin);
								m_SkinMods.push_back(mod);
							}
						}
					}
				}

				obj_ref = derived_obj->GetObjRef();
			}
		}

		this->ExtractAllJoint();
	}

	std::vector<INode*> jnodes;
	std::vector<INode*> mnodes;
	//导joint
	BOOST_FOREACH(BOOST_TYPEOF(joint_nodes_)::const_reference jn, joint_nodes_)
	{
		if (IsBone(jn.second.joint_node))
		{
			jnodes.push_back(jn.second.joint_node);
		}
	}
	BOOST_FOREACH(BOOST_TYPEOF(nodes)::const_reference node, nodes)
	{
		if (IsBone(node))
		{
			jnodes.push_back(node);
		}
		else
		{
			if (IsMesh(node))
			{
				mnodes.push_back(node);
			}
		}
	}

	std::sort(jnodes.begin(), jnodes.end());
	jnodes.erase(std::unique(jnodes.begin(), jnodes.end()), jnodes.end());
	std::sort(mnodes.begin(), mnodes.end());
	mnodes.erase(std::unique(mnodes.begin(), mnodes.end()), mnodes.end());
	//导出bone
	BOOST_FOREACH(BOOST_TYPEOF(jnodes)::const_reference jnode, jnodes)
	{
		this->ExtractBone(jnode);
	}
		
	BOOST_FOREACH(BOOST_TYPEOF(mnodes)::const_reference mnode, mnodes)
	{
		this->ExtractObject(mnode);
	}
}
//导出材质
void SE3DXMLExport::GetMaterial(materials_t& mtls, std::vector<std::map<int, std::pair<Matrix3, int> > >& uv_transss, Mtl* max_mtl)
{
	if (max_mtl)
	{
		if (0 == max_mtl->NumSubMtls())
		{
			mtls.push_back(SMaterialt());
			SMaterialt& mtl = mtls.back();
			uv_transss.push_back(std::map<int, std::pair<Matrix3, int> >());
			std::map<int, std::pair<Matrix3, int> >& uv_transs = uv_transss.back();

			mtl.ambient = max_mtl->GetAmbient();
			mtl.diffuse = max_mtl->GetDiffuse();
			mtl.specular = max_mtl->GetSpecular();
			if (max_mtl->GetSelfIllumColorOn())
			{
				mtl.emit = max_mtl->GetSelfIllumColor();
			}
			else
			{
				mtl.emit = max_mtl->GetDiffuse() * max_mtl->GetSelfIllum();
			}
			mtl.opacity = 1 - max_mtl->GetXParency();
			mtl.specular_level = max_mtl->GetShinStr();
			mtl.shininess = max_mtl->GetShininess() * 100;
			//导出贴图
			for (int j = 0; j < max_mtl->NumSubTexmaps(); ++ j)
			{
				Texmap* tex_map = max_mtl->GetSubTexmap(j);
				if ((tex_map != NULL) && (Class_ID(BMTEX_CLASS_ID, 0) == tex_map->ClassID()))
				{
					BitmapTex* bitmap_tex = static_cast<BitmapTex*>(tex_map);
					std::string map_name = tstr_to_str(bitmap_tex->GetMapName());
					if (!map_name.empty())
					{
						Matrix3 uv_mat;
						tex_map->GetUVTransform(uv_mat);

						int tex_u = 0;
						UVGen* uv_gen = tex_map->GetTheUVGen();
						if (uv_gen != NULL)
						{
							int axis = uv_gen->GetAxis();
							switch (axis)
							{
							case AXIS_UV:
								tex_u = 0;
								break;
						
							case AXIS_VW:
								tex_u = 1;
								break;

							case AXIS_WU:
								tex_u = 2;
								break;
							}
						}
						//mipp
						int channel = bitmap_tex->GetMapChannel();
						uv_transs[channel] = std::make_pair(uv_mat, tex_u);

						mtl.texture_slots.push_back(texture_slot_t(tstr_to_str(max_mtl->GetSubTexmapSlotName(j).data()), map_name));
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < max_mtl->NumSubMtls(); ++ i)
			{
				this->GetMaterial(mtls, uv_transss, max_mtl->GetSubMtl(i));
			}
		}
	}
}
//导出对象
void SE3DXMLExport::ExtractObject(INode* node)
{
	assert(IsMesh(node));

	std::string		obj_name;
	vertices_t		obj_vertices;
	triangles_t		obj_triangles;
	vertex_elements_t obj_vertex_elements;

	obj_name = tstr_to_str(node->GetName());

	Matrix3 obj_matrix = node->GetObjTMAfterWSM(m_nCurTime);
	bool flip_normals = obj_matrix.Parity() ? true : false;

	std::vector<std::pair<Point3, binds_t> > positions;
	std::map<int, std::vector<Point2> > texs;
	std::vector<int> pos_indices;
	std::map<int, std::vector<int> > tex_indices;
	std::vector<std::map<int, std::pair<Matrix3, int> > > uv_transs;

	size_t mtl_base_index = m_ObjsMtl.size();
	Mtl* mtl = node->GetMtl();
	if (mtl != NULL)
	{
		this->GetMaterial(m_ObjsMtl, uv_transs, mtl);
	}

	std::vector<unsigned int> face_sm_group;
	std::vector<unsigned int> face_mtl_id;
	std::vector<std::vector<std::vector<unsigned int> > > vertex_sm_group;

	Object* obj = node->EvalWorldState(m_nCurTime).obj;
	if ((obj != NULL) && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject* tri = static_cast<TriObject*>(obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0)));
		assert(tri != NULL);

		bool need_delete = false;
		if (obj != tri)
		{
			need_delete = true;
		}

		Mesh& mesh = tri->GetMesh();
		if (mesh.getNumFaces() > 0)
		{
			face_sm_group.resize(mesh.getNumFaces());
			face_mtl_id.resize(mesh.getNumFaces());

			obj_triangles.resize(mesh.getNumFaces());

			for (int channel = 1; channel < MAX_MESHMAPS; channel ++)
			{
				if (mesh.mapSupport(channel))
				{
					const int num_map_verts = mesh.getNumMapVerts(channel);
					if (num_map_verts > 0)
					{
						texs[channel].resize(num_map_verts, Point2(0.0f, 0.0f));

						Matrix3 tex_mat;
						tex_mat.IdentityMatrix();
						int tex_u = 0;
						for (size_t j = 0; j < uv_transs.size(); ++ j)
						{
							if (uv_transs[j].find(channel) == uv_transs[j].end())
							{
								uv_transs[j][channel] = std::make_pair(tex_mat, tex_u);
							}
						}

						UVVert* uv_verts = mesh.mapVerts(channel);
						TVFace* tv_faces = mesh.mapFaces(channel);
						for (size_t i = 0; i < obj_triangles.size(); ++ i)
						{
							int mtl_id = mesh.getFaceMtlIndex(static_cast<int>(i)) % (m_ObjsMtl.size() - mtl_base_index);

							tex_mat = uv_transs[mtl_id][channel].first;
							tex_u = uv_transs[mtl_id][channel].second;

							for (int j = 2; j >= 0; -- j)
							{
								int ti = tv_faces[i].t[j];
								tex_indices[channel].push_back(ti);

								Point3 uvw;
								// NaN test
								if ((uv_verts[ti].x != uv_verts[ti].x)
									|| (uv_verts[ti].y != uv_verts[ti].y)
									|| (uv_verts[ti].z != uv_verts[ti].z))
								{
									uvw = Point3(0, 0, 0);
								}
								else
								{
									uvw = uv_verts[ti] * tex_mat;
								}

								texs[channel][ti].x = uvw[tex_u];
								texs[channel][ti].y = uvw[(tex_u + 1) % 3];
							}
						}
					}
				}
			}

			for (int i = 0; i < mesh.getNumFaces(); ++ i)
			{
				face_sm_group[i] = mesh.faces[i].getSmGroup();
				face_mtl_id[i] = mesh.faces[i].getMatID();
				if (m_ObjsMtl.size() != mtl_base_index)
				{
					face_mtl_id[i] = static_cast<unsigned int>(mtl_base_index + face_mtl_id[i] % (m_ObjsMtl.size() - mtl_base_index));
				}
				for (int j = 2; j >= 0; -- j)
				{
					pos_indices.push_back(mesh.faces[i].v[j]);
				}
			}

			positions.resize(mesh.getNumVerts());
			vertex_sm_group.resize(mesh.getNumVerts());
			for (int i = 0; i < mesh.getNumVerts(); ++ i)
			{
				positions[i] = std::make_pair(mesh.getVert(i), binds_t());
			}
			for (int i = 0; i < mesh.getNumFaces(); ++ i)
			{
				unsigned int sm = face_sm_group[i];
				for (int j = 2; j >= 0; -- j)
				{
					int index = mesh.faces[i].v[j];
					bool found = false;
					for (size_t k = 0; k < vertex_sm_group[index].size() && !found; ++ k)
					{
						for (size_t l = 0; l < vertex_sm_group[index][k].size(); ++ l)
						{
							if (face_sm_group[vertex_sm_group[index][k][l]] & sm)
							{
								vertex_sm_group[index][k].push_back(i);
								found = true;
								break;
							}
						}
					}

					if (!found)
					{
						vertex_sm_group[index].push_back(std::vector<unsigned int>(1, i));
					}
				}
			}
		}

		if (need_delete)
		{
			delete tri;
		}
	}
	//triangle export、
	if (!obj_triangles.empty())
	{
		if (tex_indices.empty())
		{
			tex_indices[1] = pos_indices;
			texs[1].resize(positions.size(), Point2(0.0f, 0.0f));
		}
		//index
		std::set<SVertexIndex> vertex_indices;
		for (size_t i = 0; i < obj_triangles.size(); ++ i)
		{
			for (size_t j = 0; j < 3; ++ j)
			{
				SVertexIndex vertex_index;

				size_t offset;
				if (!flip_normals)
				{
					offset = i * 3 + j;
				}
				else
				{
					offset = i * 3 + (2 - j);
				}

				vertex_index.pos_index = pos_indices[offset];
				BOOST_FOREACH(BOOST_TYPEOF(tex_indices)::const_reference tex_index, tex_indices)
				{
					vertex_index.tex_indices.push_back(tex_index.second[offset]);
				}

				for (size_t k = 0; k < vertex_sm_group[vertex_index.pos_index].size(); ++ k)
				{
					for (size_t l = 0; l < vertex_sm_group[vertex_index.pos_index][k].size(); ++ l)
					{
						if (vertex_sm_group[vertex_index.pos_index][k][l] == static_cast<unsigned int>(i))
						{
							vertex_index.sm_index = static_cast<int>(k);
							break;
						}
					}
				}

				std::set<SVertexIndex>::iterator v_iter = vertex_indices.find(vertex_index);
				if (v_iter != vertex_indices.end())
				{
					// Respect set Immutability in C++0x
					vertex_index.ref_triangle = v_iter->ref_triangle;
					vertex_indices.erase(v_iter);
				}
				vertex_index.ref_triangle.push_back(i * 3 + j);
				vertex_indices.insert(vertex_index);
			}
		}

		//joint
		if (m_nJointsPerVer > 0)
		{
			Object* obj_ref = node->GetObjectRef();
			while ((obj_ref != NULL) && (GEN_DERIVOB_CLASS_ID == obj_ref->SuperClassID()))
			{
				IDerivedObject* derived_obj = static_cast<IDerivedObject*>(obj_ref);

				// Iterate over all entries of the modifier stack.
				for (int mod_stack_index = 0; mod_stack_index < derived_obj->NumModifiers(); ++ mod_stack_index)
				{
					Modifier* mod = derived_obj->GetModifier(mod_stack_index);
					if (Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B) == mod->ClassID())
					{
						this->PhysiqueModifier(mod, node, positions);
					}
					else
					{
						if (SKIN_CLASSID == mod->ClassID())
						{
							this->SkinModifier(mod, node, positions);
						}
					}
				}

				obj_ref = derived_obj->GetObjRef();
			}


			Matrix3 tm = node->GetObjTMAfterWSM(0);
			BOOST_FOREACH(BOOST_TYPEOF(positions)::reference pos_binds, positions)
			{
				if (pos_binds.second.empty())
				{
					INode* parent_node = node->GetParentNode();
					while ((parent_node != m_pRootNode) && !IsBone(parent_node))
					{
						parent_node = parent_node->GetParentNode();
					}

					pos_binds.second.push_back(std::make_pair(tstr_to_str(parent_node->GetName()), 1.0f));
				}

				Point3 v0 = pos_binds.first * tm;
				pos_binds.first = Point3(0, 0, 0);
				for (size_t i = 0 ; i < pos_binds.second.size(); ++ i)
				{
					assert(m_Joints.find(pos_binds.second[i].first) != m_Joints.end());

					pos_binds.first += pos_binds.second[i].second
						* (v0 * joint_nodes_[pos_binds.second[i].first].mesh_init_matrix);
				}

				if (pos_binds.second.size() > static_cast<size_t>(m_nJointsPerVer))
				{
					std::nth_element(pos_binds.second.begin(), pos_binds.second.begin() + m_nJointsPerVer, pos_binds.second.end(), BindCmp);
					pos_binds.second.resize(m_nJointsPerVer);

					float sum_weight = 0;
					for (int j = 0; j < m_nJointsPerVer; ++ j)
					{
						sum_weight += pos_binds.second[j].second;
					}
					assert(sum_weight > 0);

					for (int j = 0; j < m_nJointsPerVer; ++ j)
					{
						pos_binds.second[j].second /= sum_weight;
					}
				}
				else
				{
					for (int j = static_cast<int>(pos_binds.second.size()); j < m_nJointsPerVer; ++ j)
					{
						pos_binds.second.push_back(std::make_pair(tstr_to_str(m_pRootNode->GetName()), 0.0f));
					}
				}
			}
		}
		else
		{
			BOOST_FOREACH(BOOST_TYPEOF(positions)::reference pos_binds, positions)
			{
				pos_binds.first = pos_binds.first * obj_matrix;
			}
		}

		std::vector<Point3> face_normals(obj_triangles.size());
		std::vector<Point3> face_tangents(obj_triangles.size());
		std::vector<Point3> face_binormals(obj_triangles.size());
		for (size_t i = 0; i < face_normals.size(); ++ i)
		{
			face_normals[i] = ComputeNormal(positions[pos_indices[i * 3 + 2]].first,
				positions[pos_indices[i * 3 + 1]].first, positions[pos_indices[i * 3 + 0]].first);

			ComputeTangent(face_tangents[i], face_binormals[i], positions[pos_indices[i * 3 + 2]].first,
				positions[pos_indices[i * 3 + 1]].first, positions[pos_indices[i * 3 + 0]].first,
				texs[1][tex_indices[1][i * 3 + 2]], texs[1][tex_indices[1][i * 3 + 1]], texs[1][tex_indices[1][i * 3 + 0]],
				face_normals[i]);
		}
		//tangent 。。。
		obj_vertices.resize(vertex_indices.size());
		int ver_index = 0;
		BOOST_FOREACH(BOOST_TYPEOF(vertex_indices)::const_reference vertex_index, vertex_indices)
		{
			SVertex& vertex = obj_vertices[ver_index];

			vertex.pos = positions[vertex_index.pos_index].first * m_fScale;
			std::swap(vertex.pos.y, vertex.pos.z);

			Point3 normal(0, 0, 0);
			Point3 tangent(0, 0, 0);
			Point3 binormal(0, 0, 0);
			for (size_t i = 0; i < vertex_sm_group[vertex_index.pos_index][vertex_index.sm_index].size(); ++ i)
			{
				unsigned int tri_id = vertex_sm_group[vertex_index.pos_index][vertex_index.sm_index][i];
				normal += face_normals[tri_id];
				tangent += face_tangents[tri_id];
				binormal += face_binormals[tri_id];
			}
			if (flip_normals)
			{
				normal = -normal;
				tangent = -tangent;
				binormal = -binormal;
			}
			vertex.normal = normal.Normalize();
			// Gram-Schmidt orthogonalize
			vertex.tangent = (tangent - normal * (tangent % normal)).Normalize();
			// Calculate handedness
			vertex.weight = 1;
			if (((normal ^ tangent) % binormal) < 0)
			{
				vertex.weight = -1;
			}
			vertex.binormal = vertex.normal ^ vertex.tangent;

			std::swap(vertex.normal.y, vertex.normal.z);
			std::swap(vertex.tangent.y, vertex.tangent.z);
			std::swap(vertex.binormal.y, vertex.binormal.z);

			int uv_layer = 0;
			for (std::map<int, std::vector<Point2> >::iterator uv_iter = texs.begin();
				uv_iter != texs.end(); ++ uv_iter, ++ uv_layer)
			{
				Point2 tex = uv_iter->second[vertex_index.tex_indices[uv_layer]];
				obj_vertices[ver_index].tex.push_back(Point2(tex.x, 1 - tex.y));
			}

			for (size_t i = 0; i < vertex_index.ref_triangle.size(); ++ i)
			{
				obj_triangles[vertex_index.ref_triangle[i] / 3].vertex_index[vertex_index.ref_triangle[i] % 3] = ver_index;
			}

			vertex.binds = positions[vertex_index.pos_index].second;

			++ ver_index;
		}

		//导出顶点数据
		obj_vertex_elements.push_back(SElementType(ET_Position, 0, 3));
		obj_vertex_elements.push_back(SElementType(ET_Normal, 0, 3));
		obj_vertex_elements.push_back(SElementType(ET_Tangent, 0, 3));
		obj_vertex_elements.push_back(SElementType(ET_Binormal, 0, 3));
		for (size_t i = 0; i < obj_vertices[0].tex.size(); ++ i)
		{
			obj_vertex_elements.push_back(SElementType(ET_TextureCoord, static_cast<unsigned char>(i), 2));
		}
		if (!obj_vertices[0].binds.empty())
		{
			obj_vertex_elements.push_back(SElementType(ET_BlendWeight, 0, 4));
			obj_vertex_elements.push_back(SElementType(ET_BlendIndex, 0, 4));
		}

		for (size_t i = mtl_base_index; i < m_ObjsMtl.size(); ++ i)
		{
			triangles_t obj_info_tris;
			std::set<int> index_set;
			for (size_t j = 0; j < obj_triangles.size(); ++ j)
			{
				if (face_mtl_id[j] == i)
				{
					index_set.insert(obj_triangles[j].vertex_index[0]);
					index_set.insert(obj_triangles[j].vertex_index[1]);
					index_set.insert(obj_triangles[j].vertex_index[2]);

					obj_info_tris.push_back(obj_triangles[j]);
				}
			}

			if (!obj_info_tris.empty())
			{
				m_ObjsInfo.push_back(SObjectInfo());

				SObjectInfo& obj_info = m_ObjsInfo.back();
				obj_info.vertex_elements = obj_vertex_elements;

				obj_info.triangles.resize(obj_info_tris.size());

				std::map<int, int> mapping;
				int new_index = 0;
				for (std::set<int>::iterator iter = index_set.begin(); iter != index_set.end(); ++ iter, ++ new_index)
				{
					obj_info.vertices.push_back(obj_vertices[*iter]);
					mapping.insert(std::make_pair(*iter, new_index));
				}
				for (size_t j = 0; j < obj_info_tris.size(); ++ j)
				{
					obj_info.triangles[j].vertex_index[0] = mapping[obj_info_tris[j].vertex_index[0]];
					obj_info.triangles[j].vertex_index[1] = mapping[obj_info_tris[j].vertex_index[1]];
					obj_info.triangles[j].vertex_index[2] = mapping[obj_info_tris[j].vertex_index[2]];
				}
				
				if (m_ObjsMtl.size() - mtl_base_index <= 1)
				{
					obj_info.name = obj_name;
				}
				else
				{
					std::ostringstream oss;
					oss << obj_name << "__mat_" << (i - mtl_base_index);
					obj_info.name = oss.str();
				}
				obj_info.mtl_id = i;
			}
		}
	}
}
//导出骨骼动画
void SE3DXMLExport::ExtractBone(INode* node)
{
	assert(IsBone(node));
	//帧动画部分
	int tpf = GetTicksPerFrame();
	int start_tick = m_nStartFrame * tpf;
	int end_tick = m_nEndFrame * tpf;
	int tps = m_nFrameRate * tpf;

	SKeyframe kf;
	kf.joint = tstr_to_str(node->GetName());

	INode* parent_node = node->GetParentNode();
	if (!IsBone(parent_node))
	{
		parent_node = m_pRootNode;
	}

	for (int i = m_nStartFrame; i < m_nEndFrame; ++ i)
	{
		Matrix3 local_tm = node->GetNodeTM(i * tpf) * Inverse(parent_node->GetNodeTM(i * tpf));

		Quat quat = this->QuatFromMatrix(local_tm);
		Point3 pos = this->PointFromMatrix(local_tm) * m_fScale;
		kf.reals.push_back(quat);
		kf.duals.push_back(QuatTransToUDQ(quat, pos));
	}

	m_KeyFrame.push_back(kf);
}
//
Point3 SE3DXMLExport::PointFromMatrix(Matrix3 const & mat)
{
	Point3 pos(mat.GetTrans());
	std::swap(pos.y, pos.z);

	return pos;
}
//4元数变化
Quat SE3DXMLExport::QuatFromMatrix(Matrix3 const & mat)
{
	Quat quat(mat);
	std::swap(quat.y, quat.z);

	return quat;
}
//导出joint
void SE3DXMLExport::ExtractAllJoint()
{
	BOOST_FOREACH(BOOST_TYPEOF(joint_nodes_)::reference jn, joint_nodes_)
	{
		SJoint joint;

		INode* parent_node = jn.second.joint_node->GetParentNode();
		if (!IsBone(parent_node))
		{
			parent_node = m_pRootNode;
		}
		joint.parent_name = tstr_to_str(parent_node->GetName());
		//蒙皮
		Matrix3 tmp_tm;
		Matrix3 skin_init_tm;
		skin_init_tm.IdentityMatrix();
		bool found = false;
		for (size_t i = 0; i < m_Physiques.size(); ++ i)
		{
			if (MATRIX_RETURNED == m_Physiques[i]->GetInitNodeTM(jn.second.joint_node, tmp_tm))
			{
				skin_init_tm = tmp_tm;
				found = true;
				break;
			}
		}
		if (!found)
		{
			for (size_t i = 0; i < m_Skins.size(); ++ i)
			{
				if (SKIN_OK == m_Skins[i]->GetBoneInitTM(jn.second.joint_node, tmp_tm, false))
				{
					skin_init_tm = tmp_tm;
					found = true;
					break;
				}
			}
		}
		if (!found)
		{
			// fake bone假地 
			skin_init_tm = jn.second.joint_node->GetNodeTM(0);
		}

		jn.second.mesh_init_matrix = Inverse(jn.second.joint_node->GetNodeTM(0)) * skin_init_tm;

		Quat quat = this->PointFromMatrix(skin_init_tm);
		Point3 pos = this->QuatFromMatrix(skin_init_tm) * m_fScale;
		joint.real = quat;
		joint.dual = QuatTransToUDQ(quat, pos);

		m_Joints[jn.first] = joint;
	}
}
//joint权重
void SE3DXMLExport::AddJointWeight(binds_t& binds, std::string const & joint_name, float weight)
{
	if (weight > 0)
	{
		bool repeat = false;
		BOOST_FOREACH(BOOST_TYPEOF(binds)::reference bind, binds)
		{
			if (bind.first == joint_name)
			{
				bind.second += weight;
				repeat = true;
				break;
			}
		}
		if (!repeat)
		{
			binds.push_back(std::make_pair(joint_name, weight));
		}
	}
}
//physiquemodifier
void SE3DXMLExport::PhysiqueModifier(Modifier* mod, INode* node,
									std::vector<std::pair<Point3, binds_t> >& positions)
{
	assert(mod != NULL);
	// Is this Physique?
	assert(Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B) == mod->ClassID());

	IPhysiqueExport* phy_exp = static_cast<IPhysiqueExport*>(mod->GetInterface(I_PHYINTERFACE));
	if (phy_exp != NULL)
	{
		// create a ModContext Export Interface for the specific node of the Physique Modifier
		IPhyContextExport* mod_context = phy_exp->GetContextInterface(node);
		if (mod_context != NULL)
		{
			// needed by vertex interface (only Rigid supported by now)
			mod_context->ConvertToRigid(true);

			// more than a single bone per vertex
			mod_context->AllowBlending(true);

			for (int i = 0; i < mod_context->GetNumberVertices(); ++ i)
			{
				IPhyVertexExport* phy_ver_exp = mod_context->GetVertexInterface(i);
				if (phy_ver_exp != NULL)
				{
					switch (phy_ver_exp->GetVertexType())
					{
					case RIGID_NON_BLENDED_TYPE:
						{
							IPhyRigidVertex* phy_rigid_ver = static_cast<IPhyRigidVertex*>(phy_ver_exp);
							this->AddJointWeight(positions[i].second,
								tstr_to_str(phy_rigid_ver->GetNode()->GetName()), 1);
						}
						break;

					case RIGID_BLENDED_TYPE:
						{
							IPhyBlendedRigidVertex* phy_blended_rigid_ver = static_cast<IPhyBlendedRigidVertex*>(phy_ver_exp);
							for (int j = 0; j < phy_blended_rigid_ver->GetNumberNodes(); ++ j)
							{
								this->AddJointWeight(positions[i].second,
									tstr_to_str(phy_blended_rigid_ver->GetNode(j)->GetName()),
									phy_blended_rigid_ver->GetWeight(j));
							}
						}
						break;
					}
				}
			}
		}

		phy_exp->ReleaseContextInterface(mod_context);
	}

	mod->ReleaseInterface(I_PHYINTERFACE, phy_exp);
}
//蒙皮 modifier
void SE3DXMLExport::SkinModifier(Modifier* mod, INode* node,
								std::vector<std::pair<Point3, binds_t> >& positions)
{
	assert(mod != NULL);
	// Is this Skin?
	assert(SKIN_CLASSID == mod->ClassID());

	ISkin* skin = static_cast<ISkin*>(mod->GetInterface(I_SKIN));
	if (skin != NULL)
	{
		ISkinContextData* skin_cd = skin->GetContextInterface(node);
		if (skin_cd != NULL)
		{
			for (int i = 0; i < skin_cd->GetNumPoints(); ++ i)
			{
				for (int j = 0; j < skin_cd->GetNumAssignedBones(i); ++ j)
				{
					this->AddJointWeight(positions[i].second,
						tstr_to_str(skin->GetBone(skin_cd->GetAssignedBone(i, j))->GetName()),
						skin_cd->GetBoneWeight(i, j));
				}
			}
		}

		mod->ReleaseInterface(I_SKIN, skin);
	}
}
//移除沉郁
void SE3DXMLExport::RemoveRedundantJoints()
{
	std::set<std::string> joints_used;
	BOOST_FOREACH(BOOST_TYPEOF(m_ObjsInfo)::const_reference obj_info, m_ObjsInfo)
	{
		BOOST_FOREACH(BOOST_TYPEOF(obj_info.vertices)::const_reference vertex, obj_info.vertices)
		{
			BOOST_FOREACH(BOOST_TYPEOF(vertex.binds)::const_reference bind, vertex.binds)
			{
				joints_used.insert(bind.first);
			}
		}
	}

	BOOST_FOREACH(BOOST_TYPEOF(m_Joints)::const_reference joint, m_Joints)
	{
		if (joints_used.find(joint.first) != joints_used.end())
		{
			SJoint const * j = &joint.second;
			while (m_Joints.find(j->parent_name) != m_Joints.end())
			{
				joints_used.insert(j->parent_name);
				j = &m_Joints[j->parent_name];
			}
		}
	}

	for (BOOST_AUTO(iter, m_Joints.begin()); iter != m_Joints.end();)
	{
		if (joints_used.find(iter->first) == joints_used.end())
		{
			iter = m_Joints.erase(iter);
		}
		else
		{
			++ iter;
		}
	}
}

void SE3DXMLExport::RemoveRedundantMtls()
{
	std::vector<size_t> mtl_mapping(m_ObjsMtl.size());
	materials_t mtls_used;
	for (size_t i = 0; i < m_ObjsMtl.size(); ++ i)
	{
		bool found = false;
		for (size_t j = 0; j < mtls_used.size(); ++ j)
		{
			if ((mtls_used[j].ambient == m_ObjsMtl[i].ambient)
				&& (mtls_used[j].diffuse == m_ObjsMtl[i].diffuse)
				&& (mtls_used[j].specular == m_ObjsMtl[i].specular)
				&& (mtls_used[j].emit == m_ObjsMtl[i].emit)
				&& (mtls_used[j].opacity == m_ObjsMtl[i].opacity)
				&& (mtls_used[j].specular_level == m_ObjsMtl[i].specular_level)
				&& (mtls_used[j].shininess == m_ObjsMtl[i].shininess)
				&& (mtls_used[j].texture_slots == m_ObjsMtl[i].texture_slots))
			{
				mtl_mapping[i] = j;
				found = true;
				break;
			}
		}

		if (!found)
		{
			mtl_mapping[i] = mtls_used.size();
			mtls_used.push_back(m_ObjsMtl[i]);
		}
	}

	m_ObjsMtl = mtls_used;

	BOOST_FOREACH(BOOST_TYPEOF(m_ObjsInfo)::reference obj_info, m_ObjsInfo)
	{
		obj_info.mtl_id = mtl_mapping[obj_info.mtl_id];
	}
}
//优化 吧mtl 合并
void SE3DXMLExport::CombineMeshesWithSameMtl()
{
	objects_info_t opt_objs_info;
	for (size_t i = 0; i < m_ObjsMtl.size(); ++ i)
	{
		std::vector<vertex_elements_t> ves;
		std::vector<std::pair<size_t, size_t> > oids;
		for (size_t j = 0; j < m_ObjsInfo.size(); ++ j)
		{
			if (m_ObjsInfo[j].mtl_id == i)
			{
				bool found = false;
				for (size_t k = 0; k < ves.size(); ++ k)
				{
					if (ves[k] == m_ObjsInfo[j].vertex_elements)
					{
						oids.push_back(std::make_pair(j, k));
						found = true;
						break;
					}
				}

				if (!found)
				{
					oids.push_back(std::make_pair(j, ves.size()));
					ves.push_back(m_ObjsInfo[j].vertex_elements);
				}
			}
		}

		for (size_t j = 0; j < ves.size(); ++ j)
		{
			opt_objs_info.push_back(SObjectInfo());
			SObjectInfo& opt_obj = opt_objs_info.back();

			std::ostringstream oss;
			oss << "mesh_for_mtl_" << i << "_ve_" << j;
			opt_obj.name = oss.str();
			opt_obj.mtl_id = i;
			opt_obj.vertex_elements = ves[j];

			BOOST_FOREACH(BOOST_TYPEOF(oids)::reference oid, oids)
			{
				int base = static_cast<int>(opt_obj.vertices.size());
				if (oid.second == j)
				{
					opt_obj.vertices.insert(opt_obj.vertices.end(),
						m_ObjsInfo[oid.first].vertices.begin(), m_ObjsInfo[oid.first].vertices.end());

					BOOST_FOREACH(BOOST_TYPEOF(m_ObjsInfo[oid.first].triangles)::reference old_tri, m_ObjsInfo[oid.first].triangles)
					{
						STriangle tri = old_tri;
						tri.vertex_index[0] += base;
						tri.vertex_index[1] += base;
						tri.vertex_index[2] += base;

						opt_obj.triangles.push_back(tri);
					}
				}
			}
		}
	}

	m_ObjsInfo = opt_objs_info;
}
//排序按mtl
void SE3DXMLExport::SortMeshesByMtl()
{
	std::vector<std::pair<size_t, size_t> > mtl_ids(m_ObjsInfo.size());
	for (size_t i = 0; i < m_ObjsInfo.size(); ++ i)
	{
		mtl_ids[i].first = m_ObjsInfo[i].mtl_id;
		mtl_ids[i].second = i;
	}

	std::sort(mtl_ids.begin(), mtl_ids.end());

	objects_info_t opt_objs_info;
	for (size_t i = 0; i < mtl_ids.size(); ++ i)
	{
		opt_objs_info.push_back(m_ObjsInfo[mtl_ids[i].second]);
	}

	m_ObjsInfo = opt_objs_info;
}
//写入xml
void SE3DXMLExport::WriteXml(std::string const & file_name, SExportAttrs const & eva)
{
	std::ofstream ofs(file_name.c_str());
	if (!ofs)
	{
		return;
	}

	this->RemoveRedundantJoints();
	this->RemoveRedundantMtls();
	if (m_bCombineMeshes)
	{
		this->CombineMeshesWithSameMtl();
	}
	else
	{
		this->SortMeshesByMtl();
	}

	std::map<std::string, int> joints_name_to_id;
	std::vector<std::string> joints_id_to_name;
	{
		BOOST_FOREACH(BOOST_TYPEOF(m_Joints)::const_reference joint, m_Joints)
		{
			joints_id_to_name.push_back(joint.first);
		}

		bool swapped = true;
		while (swapped)
		{
			swapped = false;
			for (int i = 0; i < static_cast<int>(joints_id_to_name.size()); ++ i)
			{
				int par_index = -1;
				if (!m_Joints[joints_id_to_name[i]].parent_name.empty())
				{
					std::vector<std::string>::iterator par_iter = std::find(joints_id_to_name.begin(), joints_id_to_name.end(),
						m_Joints[joints_id_to_name[i]].parent_name);
					assert(par_iter != joints_id_to_name.end());
					par_index = static_cast<int>(par_iter - joints_id_to_name.begin());
				}

				if (par_index > i)
				{
					std::swap(joints_id_to_name[i], joints_id_to_name[par_index]);
					swapped = true;
					break;
				}
			}
		}

		for (int i = 0; i < static_cast<int>(joints_id_to_name.size()); ++ i)
		{
			joints_name_to_id.insert(std::make_pair(joints_id_to_name[i], i));
		}
	}

	using std::endl;

	ofs << "<?xml version=\"1.0\"?>" << endl << endl;
	ofs << "<model version=\"5\">" << endl;

	if (m_nJointsPerVer > 0)
	{
		ofs << "\t<bones_chunk>" << endl;
		BOOST_FOREACH(BOOST_TYPEOF(joints_id_to_name)::const_reference joint_name, joints_id_to_name)
		{
			int parent_id = -1;
			if (!m_Joints[joint_name].parent_name.empty())
			{
				assert(joints_name_to_id.find(m_Joints[joint_name].parent_name) != joints_name_to_id.end());
				parent_id = joints_name_to_id[m_Joints[joint_name].parent_name];
				assert(parent_id < joints_name_to_id[joint_name]);
			}

			ofs << "\t\t<bone name=\"" << joint_name
				<< "\" parent=\"" << parent_id
				<< "\">" << endl;

			SJoint const & joint = m_Joints[joint_name];

			ofs << "\t\t\t<bind_real x=\"" << joint.real.x
				<< "\" y=\"" << joint.real.y
				<< "\" z=\"" << joint.real.z
				<< "\" w=\"" << joint.real.w << "\"/>" << endl;
			ofs << "\t\t\t<bind_dual x=\"" << joint.dual.x
				<< "\" y=\"" << joint.dual.y
				<< "\" z=\"" << joint.dual.z
				<< "\" w=\"" << joint.dual.w << "\"/>" << endl;

			ofs << "\t\t</bone>" << endl;
		}
		ofs << "\t</bones_chunk>" << endl;
	}

	if (m_ObjsMtl.size() > 0)
	{
		ofs << "\t<materials_chunk>" << endl;
		for (size_t i = 0; i < m_ObjsMtl.size(); ++ i)
		{
			ofs << "\t\t<material ambient_r=\"" << m_ObjsMtl[i].ambient.r
				<< "\" ambient_g=\"" << m_ObjsMtl[i].ambient.g
				<< "\" ambient_b=\"" << m_ObjsMtl[i].ambient.b
				<< "\" diffuse_r=\"" << m_ObjsMtl[i].diffuse.r
				<< "\" diffuse_g=\"" << m_ObjsMtl[i].diffuse.g
				<< "\" diffuse_b=\"" << m_ObjsMtl[i].diffuse.b
				<< "\" specular_r=\"" << m_ObjsMtl[i].specular.r
				<< "\" specular_g=\"" << m_ObjsMtl[i].specular.g
				<< "\" specular_b=\"" << m_ObjsMtl[i].specular.b
				<< "\" emit_r=\"" << m_ObjsMtl[i].emit.r
				<< "\" emit_g=\"" << m_ObjsMtl[i].emit.g
				<< "\" emit_b=\"" << m_ObjsMtl[i].emit.b
				<< "\" opacity=\"" << m_ObjsMtl[i].opacity
				<< "\" specular_level=\"" << m_ObjsMtl[i].specular_level
				<< "\" shininess=\"" << m_ObjsMtl[i].shininess
				<< "\">" << endl;
			if (m_ObjsMtl[i].texture_slots.size() > 0)
			{
				ofs << "\t\t\t<textures_chunk>" << endl;
				BOOST_FOREACH(BOOST_TYPEOF(m_ObjsMtl[i].texture_slots)::const_reference ts, m_ObjsMtl[i].texture_slots)
				{
					ofs << "\t\t\t\t<texture type=\"" << ts.first
						<< "\" name=\"" << ts.second << "\"/>" << endl;
				}
				ofs << "\t\t\t</textures_chunk>" << endl;
			}
			ofs << "\t\t</material>" << endl;
		}
		ofs << "\t</materials_chunk>" << endl;
	}

	ofs << "\t<meshes_chunk>" << endl;
	BOOST_FOREACH(BOOST_TYPEOF(m_ObjsInfo)::const_reference obj_info, m_ObjsInfo)
	{
		ofs << "\t\t<mesh name=\"" << obj_info.name << "\" mtl_id=\"" << obj_info.mtl_id << "\">" << endl;

		ofs << "\t\t\t<vertices_chunk>" << endl;
		BOOST_FOREACH(BOOST_TYPEOF(obj_info.vertices)::const_reference vertex, obj_info.vertices)
		{
			ofs << "\t\t\t\t<vertex x=\"" << vertex.pos.x
				<< "\" y=\"" << vertex.pos.y
				<< "\" z=\"" << vertex.pos.z << "\">" << endl;

			if (eva.normal)
			{
				ofs << "\t\t\t\t\t<normal x=\"" << vertex.normal.x
					<< "\" y=\"" << vertex.normal.y
					<< "\" z=\"" << vertex.normal.z << "\"/>" << endl;
			}
			if (eva.tangent)
			{
				ofs << "\t\t\t\t\t<tangent x=\"" << vertex.tangent.x
					<< "\" y=\"" << vertex.tangent.y
					<< "\" z=\"" << vertex.tangent.z;
				if (vertex.weight < 0)
				{
					ofs << "\" w=\"" << vertex.weight;
				}
				ofs << "\"/>" << endl;
			}
			if (eva.binormal)
			{
				ofs << "\t\t\t\t\t<binormal x=\"" << vertex.binormal.x
					<< "\" y=\"" << vertex.binormal.y
					<< "\" z=\"" << vertex.binormal.z << "\"/>" << endl;
			}

			if (eva.tex)
			{
				BOOST_FOREACH(BOOST_TYPEOF(vertex.tex)::const_reference tex, vertex.tex)
				{
					ofs << "\t\t\t\t\t<tex_coord u=\"" << tex.x
						<< "\" v=\"" << tex.y << "\"/>" << endl;
				}
			}

			BOOST_FOREACH(BOOST_TYPEOF(vertex.binds)::const_reference bind, vertex.binds)
			{
				assert(joints_name_to_id.find(bind.first) != joints_name_to_id.end());
				ofs << "\t\t\t\t\t<weight bone_index=\"" << joints_name_to_id[bind.first]
					<< "\" weight=\"" << bind.second << "\"/>" << endl;
			}

			ofs << "\t\t\t\t</vertex>" << endl;
		}
		ofs << "\t\t\t</vertices_chunk>" << endl;

		ofs << "\t\t\t<triangles_chunk>" << endl;
		BOOST_FOREACH(BOOST_TYPEOF(obj_info.triangles)::const_reference tri, obj_info.triangles)
		{
			ofs << "\t\t\t\t<triangle a=\"" << tri.vertex_index[0]
				<< "\" b=\"" << tri.vertex_index[1]
				<< "\" c=\"" << tri.vertex_index[2] << "\"/>" << endl;
		}
		ofs << "\t\t\t</triangles_chunk>" << endl;

		ofs << "\t\t</mesh>" << endl;
	}
	ofs << "\t</meshes_chunk>" << endl;

	if (m_nJointsPerVer > 0)
	{
		ofs << "\t<key_frames_chunk start_frame=\"" << m_nStartFrame
			<< "\" end_frame=\"" << m_nEndFrame
			<< "\" frame_rate=\"" << m_nFrameRate << "\">" << endl;
		BOOST_FOREACH(BOOST_TYPEOF(m_KeyFrame)::const_reference kf, m_KeyFrame)
		{
			assert(kf.reals.size() == kf.duals.size());

			ofs << "\t\t<key_frame joint=\"" << kf.joint << "\">" << endl;

			for (size_t i = 0; i < kf.reals.size(); ++ i)
			{
				ofs << "\t\t\t<key>" << endl;

				ofs << "\t\t\t\t<bind_real x=\"" << kf.reals[i].x
					<< "\" y=\"" << kf.reals[i].y
					<< "\" z=\"" << kf.reals[i].z
					<< "\" w=\"" << kf.reals[i].w << "\"/>" << endl;
				ofs << "\t\t\t\t<bind_dual x=\"" << kf.duals[i].x
					<< "\" y=\"" << kf.duals[i].y
					<< "\" z=\"" << kf.duals[i].z
					<< "\" w=\"" << kf.duals[i].w << "\"/>" << endl;

				ofs << "\t\t\t</key>" << endl;
			}

			ofs << "\t\t</key_frame>" << endl;
		}
		ofs << "\t</key_frames_chunk>" << endl;
	}

	ofs << "</model>" << endl;
}

