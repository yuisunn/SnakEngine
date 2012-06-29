//碰撞检测体
// 2011 12 222
////////////////////////
#ifndef _BoundingGeo_H_
#define _BoundingGeo_H_
#include <SE3DEngine/Comm/Def.h>
#include <SEMath/Vector4.h>

namespace SE
{
struct AABB
{
	Vector4 min;
	Vector4 max;
	enum type_reset { RESET };
	//初始化
	inline AABB()
	{}
		//重置 重置检查
	inline void Rest()
	{
		//1e35f ???不懂 10进制=123743
		min = Vector4(1e35f); max = Vector4(-1e35f);
	}
	inline bool IsReset() const
	{
		return min.x > max.y;
	}
	inline AABB(type_reset)
	{
		Rest();
	}
	inline AABB( Vector4&v)
	{
		 min = -v; max = v;
	}
	inline  AABB(  Vector4  &vmin,  Vector4  &vmax ) 
	{ 
		min=vmin; max=vmax; 
	}
	inline  AABB( const AABB &aabb ) 
	{
		min.x = aabb.min.x;			min.y = aabb.min.y;			min.z = aabb.min.z;
		max.x = aabb.max.x;			max.y = aabb.max.y;			max.z = aabb.max.z;
	}
	//ILINE AABB( Vector4 const &points, Vector4 num)
	//{
	//	Rest();
	//	for(uint32 i=0; i<num; i++)
	//	    Add(points[i]);
	//}
	//操作符重载
	inline void operator() (const Vector4 &vmin, const Vector4 &vmax)
	{
		min = vmin; max = vmax;
	}
	inline Vector4 Center() 
	{
		return (min + max)/2.0f;
	}
	inline Vector4 HalfSize()
	{
		return (max - min) / 2.0f;
	}

	inline float IsResetSel(float ifReset, float ifNotReset) const 
	{
		//return (float)__fsel(max.x - min.x, ifNotReset, ifReset); 
		 return ((max.x-min.x)<0.0f) ? ifReset : ifNotReset;
	}
	//检测空
	inline bool IsEmpty()  
	{ 
		return min == max;
	}
	inline Vector4 GetCenter()  
	{ 
		return (min+max)*0.5f; 
	}
	inline Vector4 GetSize()  
	{ 
		return (max-min) * IsResetSel(0.0f, 1.0f);
	}
	inline float GetRadius()  
	{
		return IsResetSel(0.0f, (max-min).Length()*0.5f);
	}
	//length 计算没 sqr
	//ILINE float GetRadiusSqr() const 
	//{
	//	return IsResetSel(0.0f, ((max-min)*0.5f).Length());
	//}
	inline float GetVolume() const 
	{ 
		return IsResetSel(0.0f, (max.x-min.x) * (max.y-min.y) * (max.z-min.z));
	}
	//添加vec
	void Add(Vector4 & v )
	{
		min.CheckMin(v);
        max.CheckMax(v);
	}
	//增加范围
	inline void Add( Vector4 & v, float fRadius )
	{
		Vector4 vRad(fRadius,fRadius,fRadius);
		min.CheckMin(v-vRad);
		max.CheckMax(v+vRad);
	}
	//和林一个aabb比较
	inline void Add(  AABB & bb )
	{
		min.CheckMin(bb.min);
		max.CheckMax(bb.max);
	}
	//移动
	inline void Move(  Vector4 &v )
	{
		float moveMult = IsResetSel(0.0f, 1.0f);
	    Vector4 vMove = v * moveMult;
		min += vMove;
		max += vMove;
	}
	//const &  不能用作实参数
	//const int 不能做 + -
	inline void Expand( Vector4& v )
	{
		if (!IsReset())
		{
			min -= v;
			max += v;
		}
	}

	//扩充
	inline void Augment( AABB& bb )
	{
		if (!IsReset() && !bb.IsReset())
		{
			Add( min + bb.min );
			Add( max + bb.max );
		}
	}
	//box 和box 检测
	inline void ClipToBox( AABB& bb )
	{
		min.CheckMax(bb.min);
		max.CheckMin(bb.max);
	}
	//
	inline void ClipMoveToBox( AABB& bb )
	{
		for (int a = 0; a < 3; a++)
		{
			if (max[a] - min[a] > bb.max[a] - bb.min[a])
			{
				min[a] = bb.min[a];  
				max[a] = bb.max[a];
			}
			else if (min[a] < bb.min[a])
			{
				max[a] += bb.min[a] - min[a];
				min[a] = bb.min[a];
			}
			else if (max[a] > bb.max[a])
			{
				min[a] += bb.max[a] - max[a];
				max[a] = bb.max[a];
			}
		}
	}
	inline AABB& operator |=(AABB& rhs)
	{
		min = this->min.CheckMin(rhs.min);
		max = this->max.CheckMin(rhs.max);
		return *this;
	}




};



}
#endif