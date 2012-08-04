#ifndef _Quaternion_H_
#define _Quaternion_H_

// Vector library
// Peter Pon Aug 30 2006

#include <assert.h>
#include <math.h>

#include <SEMath/FastMath.h>

class Quaternion;

extern Quaternion g_Quaternion_Zero;
extern Quaternion g_Quaternion_One;
extern Quaternion g_Quaternion_Identity;

class Quaternion
{
public:
	union{
		struct 
		{
			float x,y,z,w;
		};
		float m_elem[4];
		unsigned int m_ielem[4];
	};

public:
	inline Quaternion(void)
	{
		// do nothing
	}

	inline Quaternion(float x , float y , float z , float w=1.0f)
	{
		Set(x, y, z, w);
	}

	inline Quaternion(float value)
	{
		Set(value);
	}

	inline Quaternion(unsigned int x, unsigned int y, unsigned int z, unsigned int w)
	{
		m_ielem[0] = x;
		m_ielem[0] = y;
		m_ielem[0] = z;
		m_ielem[0] = w;
	}

	inline Quaternion(unsigned int x)
	{
		m_ielem[0] = x;
		m_ielem[0] = x;
		m_ielem[0] = x;
		m_ielem[0] = x;
	}

	void ConsoleOutput(void);

	inline void Set(float x , float y , float z , float w )
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void Set(float value)
	{
		this->w = value;
		this->x = value;
		this->y = value;
		this->z = value;	
	}

	inline void Set(float *p)
	{
		
		this->x = p[0];
		this->y = p[1];
		this->z = p[2];
		this->w = p[3];
	}

	inline void SetX(float x)
	{
		this->x = x;
	}

	inline void SetY(float y)
	{
		this->y = y;
	}

	inline void SetZ(float z)
	{
		this->z = z;
	}

	inline void SetW(float w)
	{
		this->w = w;
	}

	inline void SetXYZ(float *p)
	{
		x = p[0];
		y = p[1];
		z = p[2];
	}

	inline float GetX(void)
	{
		return x;
	}

	inline Quaternion GetXXXX(void)
	{
		Quaternion result(x);
		return result;
	}

	inline float GetY(void)
	{
		return y;
	}

	inline Quaternion GetYYYY(void)
	{
		Quaternion result(y);
		return result;
	}

	inline float GetZ(void)
	{
		return z;
	}

	inline Quaternion GetZZZZ(void)
	{
		Quaternion result(z);
		return result;
	}

	inline float GetW(void)
	{
		return w;
	}

	inline Quaternion GetWWWW(void)
	{
		Quaternion result(w);
		return result;
	}

	inline float StoreAligned(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = w;
	}

	inline void Store(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = w;
	}

	inline void StoreXYZ(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
	}

	inline const Quaternion Identity()
	{
		 return g_Quaternion_Identity;
	}

	inline float Length(void)
	{
		return  x*x + y*y + z*z + w*w;
	}

	inline void Normalize(void)
	{
		float len = Length();
		x/=len;
		y/=len;
		z/=len;
	}

	inline float NormalizeAndGetLength(void)
	{
		float len = Length();
		x/=len;
		y/=len;
		z/=len;
		return len;
	}

	inline Quaternion &Abs(void)
	{
		x = SEMath::Abs(x);
		y = SEMath::Abs(y);
		z = SEMath::Abs(z);
		w = SEMath::Abs(w);

		return *this;
	}

	inline Quaternion &InvertSign(void)
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}

	inline Quaternion &Saturate(void)
	{
		if ( x > 1.0f ) x = 1.0f;
		if ( y > 1.0f ) y = 1.0f;
		if ( z > 1.0f ) z = 1.0f;
		if ( w > 1.0f ) w = 1.0f;

		if ( x < 0.0f ) x = 0.0f;
		if ( y < 0.0f ) y = 0.0f;
		if ( z < 0.0f ) z = 0.0f;
		if ( w < 0.0f ) w = 0.0f;
	
		return *this;
	}

	inline Quaternion &Clamp_to_0(void)
	{
		if ( x < 0.0f ) x = 0.0f;
		if ( y < 0.0f ) y = 0.0f;
		if ( z < 0.0f ) z = 0.0f;
		if ( w < 0.0f ) w = 0.0f;
	
		return *this;
	}

	inline Quaternion &Clamp_to_1(void)
	{
		if ( x > 1.0f ) x = 1.0f;
		if ( y > 1.0f ) y = 1.0f;
		if ( z > 1.0f ) z = 1.0f;
		if ( w > 1.0f ) w = 1.0f;

		return *this;
	}

	inline Quaternion &Floor(void)
	{
		x = floor(x);
		y = floor(y);
		z = floor(z);
		w = floor(w);
		return *this;
	}

	inline static Quaternion &GetZero(void)
	{
		return g_Quaternion_Zero;
	}

	inline static Quaternion &GetOne(void)
	{
		return g_Quaternion_One;
	}

	// operators
	inline Quaternion &operator=(Quaternion &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline Quaternion &operator=(float value)
	{
		Set(value);
		return *this;
	}

	inline Quaternion &operator+=(Quaternion &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	inline Quaternion &operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
		w += value;
		return *this;
	}

	inline Quaternion &operator-=(Quaternion &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline Quaternion &operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;
		w -= value;
		return *this;
	}
	
	inline Quaternion &operator*=(Quaternion &rhs)
	{
		///? 四元数 乘法是叉乘
		*this = Quaternion(x*rhs.w - y*rhs.z + z*rhs.y + w*rhs.x,
			              x*rhs.z + y*rhs.w - z*rhs.x + w*rhs.y,
						  y*rhs.x - x*rhs.y + z*rhs.w + w*rhs.z,
						  w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
		return *this;
	}





	inline float &operator[](int index)
	{
		assert(index>=0 && index<4);
		return m_elem[index];
	}

	static const char *InstructionType(void)
	{
		return "General_CPU";
	}
};

inline bool operator==(Quaternion &a, Quaternion &b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z && a.w==b.w);
	return result;
}

inline bool operator!=(Quaternion &a, Quaternion &b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z && a.w==b.w);
	return !result;
}

inline Quaternion operator+(Quaternion &a, Quaternion &b)
{
	Quaternion c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;

	return c;
}

inline Quaternion operator-(Quaternion &a, Quaternion &b)
{
	Quaternion c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;

	return c;
}
inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	result.x = lhs.x*rhs.w - lhs.y*rhs.z + lhs.z*rhs.y + lhs.w*rhs.x;
	result.y = lhs.x*rhs.z + lhs.y*rhs.w - lhs.z*rhs.x + lhs.w*rhs.y;
	result.z = lhs.y*rhs.x - lhs.x*rhs.y + lhs.z*rhs.w + lhs.w*rhs.z;
	result.w = lhs.w*rhs.w - lhs.x*rhs.x - lhs.y*rhs.y - lhs.z*rhs.z;
	return result;
}






#endif // _VECTOR4_SSE_