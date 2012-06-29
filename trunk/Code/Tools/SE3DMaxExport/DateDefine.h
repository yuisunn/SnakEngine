#ifndef _DateDefine_H_
#define _DateDefine_H_

#include <vector>
#include <string>
#include <map>


enum EElementType
{
	// vertex positions
	ET_Position,
	// vertex normals included (for lighting)
	ET_Normal,
	// Vertex colors - diffuse
	ET_Diffuse,
	// Vertex colors - specular
	ET_Specular,
	// Vertex blend weights
	ET_BlendWeight,
	// Vertex blend indices
	ET_BlendIndex,
	// at least one set of texture coords (exact number specified in class)
	ET_TextureCoord,
	// Vertex tangent
	ET_Tangent,
	// Vertex binormal
	ET_Binormal
};

struct SElementType
{
	SElementType(EElementType usage, unsigned char usage_index, unsigned char num_components)
		: usage(usage), usage_index(usage_index), num_components(num_components)
	{
	}

	EElementType usage;
	unsigned char usage_index;
	unsigned char num_components;

	friend bool operator==(SElementType const & lhs, SElementType const & rhs)
	{
		return (lhs.usage == rhs.usage) && (lhs.usage_index == rhs.usage_index) && (lhs.num_components == rhs.num_components);
	}
	friend bool operator!=(SElementType const & lhs, SElementType const & rhs)
	{
		return !(lhs == rhs);
	}
};

typedef std::vector<std::pair<std::string, float> > binds_t;

struct SVertex
{
	Point3 pos;
	Point3 normal;
	Point3 tangent;
	float weight;
	Point3 binormal;
	std::vector<Point2> tex;

	binds_t binds;
};


struct SVertexIndex
{
	int pos_index;
	std::vector<int> tex_indices;
	int sm_index;

	std::vector<size_t> ref_triangle;
};

typedef std::vector<SVertex> vertices_t;

struct STriangle
{
	int	vertex_index[3];
};

typedef std::vector<STriangle> triangles_t;

typedef std::pair<std::string, std::string> texture_slot_t;
typedef std::vector<texture_slot_t> texture_slots_t;

struct SMaterialt
{
	Color ambient;
	Color diffuse;
	Color specular;
	Color emit;
	float opacity;
	float specular_level;
	float shininess;

	texture_slots_t texture_slots;
};
typedef std::vector<SMaterialt> materials_t;

typedef std::vector<SElementType> vertex_elements_t;

struct SJoint
{
	Quat real;
	Quat dual;

	std::string parent_name;
};

typedef std::map<std::string, SJoint> joints_t;

struct SKeyframe 
{
	std::string joint;

	std::vector<Quat> reals;
	std::vector<Quat> duals;
};

typedef std::vector<SKeyframe> key_frames_t;

struct SObjectInfo
{
	std::string		name;

	size_t			mtl_id;

	vertices_t		vertices;
	triangles_t		triangles;

	vertex_elements_t vertex_elements;
};

struct SJointMat
{
	INode* joint_node;
	Matrix3 mesh_init_matrix;
};

struct SExportAttrs
{
	bool normal;
	bool tangent;
	bool binormal;
	bool tex;
};


////²Ù×÷·û
//bool operator<(SVertexIndex const & lhs, SVertexIndex const & rhs)
//{
//	if (lhs.pos_index < rhs.pos_index)
//	{
//		return true;
//	}
//	else
//	{
//		if (lhs.pos_index > rhs.pos_index)
//		{
//			return false;
//		}
//		else
//		{
//			if (lhs.tex_indices < rhs.tex_indices)
//			{
//				return true;
//			}
//			else
//			{
//				if (lhs.tex_indices > rhs.tex_indices)
//				{
//					return false;
//				}
//				else
//				{
//					if (lhs.sm_index < rhs.sm_index)
//					{
//						return true;
//					}
//					else
//					{
//						return false;
//					}
//				}
//			}
//		}
//	}
//}
//
//struct less_Point2 : public std::binary_function<Point2, Point2, bool>
//{
//	bool operator()(Point2 const & lhs, Point2 const & rhs) const
//	{
//		if (lhs.x < rhs.x)
//		{
//			return true;
//		}
//		else
//		{
//			if (lhs.x > rhs.x)
//			{
//				return false;
//			}
//			else
//			{
//				if (lhs.y < rhs.y)
//				{
//					return true;
//				}
//				else
//				{
//					return false;
//				}
//			}
//		}
//	}
//};
//
//bool BindCmp(std::pair<std::string, float> const& lhs,
//	std::pair<std::string, float> const& rhs)
//{
//	return lhs.second > rhs.second;
//}
//
//Point3 ComputeNormal(Point3 const & v0XYZ, Point3 const & v1XYZ, Point3 const & v2XYZ)
//{
//	Point3 v1v0 = v1XYZ - v0XYZ;
//	Point3 v2v0 = v2XYZ - v0XYZ;
//
//	return CrossProd(v1v0, v2v0);
//}
//
//void ComputeTangent(Point3& tangent, Point3& binormal, Point3 const & v0XYZ, Point3 const & v1XYZ, Point3 const & v2XYZ,
//	Point2 const & v0Tex, Point2 const & v1Tex, Point2 const & v2Tex,
//	Point3 const & normal)
//{
//	Point3 v1v0 = v1XYZ - v0XYZ;
//	Point3 v2v0 = v2XYZ - v0XYZ;
//
//	float s1 = v1Tex.x - v0Tex.x;
//	float t1 = v1Tex.y - v0Tex.y;
//
//	float s2 = v2Tex.x - v0Tex.x;
//	float t2 = v2Tex.y - v0Tex.y;
//
//	float denominator = s1 * t2 - s2 * t1;
//	if (abs(denominator) < std::numeric_limits<float>::epsilon())
//	{
//		tangent = Point3(1, 0, 0);
//		binormal = Point3(0, 1, 0);
//	}
//	else
//	{
//		tangent = (t2 * v1v0 - t1 * v2v0) / denominator;
//		binormal = (s1 * v2v0 - s2 * v1v0) / denominator;
//	}
//}
//
//Quat QuatTransToUDQ(Quat const & q, Point3 const & t)
//{
//	return Quat(+0.5f * (+t[0] * q[3] + t[1] * q[2] - t[2] * q[1]),
//		+0.5f * (-t[0] * q[2] + t[1] * q[3] + t[2] * q[0]),
//		+0.5f * (+t[0] * q[1] - t[1] * q[0] + t[2] * q[3]),
//		-0.5f * (+t[0] * q[0] + t[1] * q[1] + t[2] * q[2]));
//}
//


#endif
