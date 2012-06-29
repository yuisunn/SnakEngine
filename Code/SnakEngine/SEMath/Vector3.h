#ifndef _VECTOR3_GENERAL_
#define _VECTOR3_GENERAL_

// Vector library
// Peter Pon Aug 30 2006

#include <assert.h>
#include <math.h>

#include <SEMath/FastMath.h>

class Vector3;

extern Vector3 g_Vector3CPU_Zero;
extern Vector3 g_Vector3CPU_One;

class  Vector3
{
public:
	union{
		struct 
		{
			float x,y,z;
		};
		float m_elem[3];
		unsigned int m_ielem[3];
	};

public:
	inline Vector3(void)
	{
		// do nothing
	}

	inline Vector3(float const& x, float const& y, float const& z)
	{
		Set(x, y, z);
	}

	inline Vector3(float const& value)
	{
		Set(value);
	}

	inline Vector3(unsigned int x, unsigned int y, unsigned int z)
	{
		m_ielem[0] = x;
		m_ielem[0] = y;
		m_ielem[0] = z;
	}

	inline Vector3(unsigned int x)
	{
		m_ielem[0] = x;
		m_ielem[0] = x;
		m_ielem[0] = x;
		m_ielem[0] = x;
	}

	void ConsoleOutput(void);

	inline void Set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void Set(float value)
	{
		this->x = value;
		this->y = value;
		this->z = value;
	}

	inline void Set(float *p)
	{
		this->x = p[0];
		this->y = p[1];
		this->z = p[2];
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

	inline Vector3 GetXXX(void)
	{
		Vector3 result(x);
		return result;
	}

	inline float GetY(void)
	{
		return y;
	}

	inline Vector3 GetYYY(void)
	{
		Vector3 result(y);
		return result;
	}

	inline float GetZ(void)
	{
		return z;
	}

	inline Vector3 GetZZZ(void)
	{
		Vector3 result(z);
		return result;
	}

	inline float StoreAligned(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
	}

	inline void Store(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
	}

	inline void StoreXYZ(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
	}

	inline void Reciprocal(void)
	{
		x = 1.0f/x;
		y = 1.0f/y;
		z = 1.0f/z;
	}

	inline void Sqrt(void)
	{
		x = SEMath::Sqrt(x);
		y = SEMath::Sqrt(y);
		z = SEMath::Sqrt(z);
	}
	
	inline void ReciprocalSqrt(void)
	{
		x = SEMath::ReciprocalSqrt(x);
		y = SEMath::ReciprocalSqrt(y);
		z = SEMath::ReciprocalSqrt(z);
	}

	inline float Length(void)
	{
		float vectorlength;
		
		float sum = x*x + y*y + z*z;
		vectorlength = SEMath::Sqrt(sum);

		return vectorlength;
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

	inline Vector3 &Abs(void)
	{
		x = SEMath::Abs(x);
		y = SEMath::Abs(y);
		z = SEMath::Abs(z);

		return *this;
	}

	inline Vector3 &InvertSign(void)
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}

	inline Vector3 &Saturate(void)
	{
		if ( x > 1.0f ) x = 1.0f;
		if ( y > 1.0f ) y = 1.0f;
		if ( z > 1.0f ) z = 1.0f;

		if ( x < 0.0f ) x = 0.0f;
		if ( y < 0.0f ) y = 0.0f;
		if ( z < 0.0f ) z = 0.0f;
	
		return *this;
	}

	inline Vector3 &Clamp_to_0(void)
	{
		if ( x < 0.0f ) x = 0.0f;
		if ( y < 0.0f ) y = 0.0f;
		if ( z < 0.0f ) z = 0.0f;
	
		return *this;
	}

	inline Vector3 &Clamp_to_1(void)
	{
		if ( x > 1.0f ) x = 1.0f;
		if ( y > 1.0f ) y = 1.0f;
		if ( z > 1.0f ) z = 1.0f;

		return *this;
	}

	inline Vector3 &Floor(void)
	{
		x = floor(x);
		y = floor(y);
		z = floor(z);
		return *this;
	}

	inline static Vector3 &GetZero(void)
	{
		return g_Vector3CPU_Zero;
	}

	inline static Vector3 &GetOne(void)
	{
		return g_Vector3CPU_One;
	}

	// operators
	inline Vector3 &operator=(Vector3 const& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	inline Vector3 &operator=(float value)
	{
		Set(value);
		return *this;
	}

	inline Vector3 &operator!=(Vector3 &rhs)
	{

	}

	inline Vector3 &operator+=(Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline Vector3 &operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
		return *this;
	}

	inline Vector3 &operator-=(Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	inline Vector3 &operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}

	inline Vector3 &operator*=(Vector3 &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	inline Vector3 &operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}

	inline Vector3 &operator/=(Vector3 &rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
	
		return *this;
	}

	inline Vector3 &operator/=(float value)
	{
		x /= value;
		y /= value;
		z /= value;

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

inline bool operator==(Vector3 &a, Vector3 &b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z );
	return result;
}

inline bool operator!=(Vector3 &a, Vector3 &b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z );
	return !result;
}

inline bool operator!=(Vector3 const&a, Vector3 const&b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z);
	return !result;
}

inline Vector3 operator+(Vector3 &a, Vector3 &b)
{
	Vector3 c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

	return c;
}

inline Vector3 operator-(Vector3 &a, Vector3 &b)
{
	Vector3 c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;

	return c;
}

inline Vector3 operator*(Vector3 &a, Vector3 &b)
{
	Vector3 c;

	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;

	return c;
}

inline Vector3 operator*(float f, Vector3 &v)
{
	Vector3 result;

	result.x = f * v.x;
	result.y = f * v.y;
	result.z = f * v.z;


	return result;
}

inline Vector3 operator*(Vector3 &v, float f)
{
	Vector3 result;

	result.x = f * v.x;
	result.y = f * v.y;
	result.z = f * v.z;

	return result;
}

inline Vector3 operator/(Vector3 &a, Vector3 &b)
{
	Vector3 c;

	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;

	return c;
}

inline Vector3 operator/(Vector3 &a, float f)
{
	Vector3 c;

	c.x = a.x / f;
	c.y = a.y / f;
	c.z = a.z / f;

	return c;
}

inline Vector3 operator/(float f, Vector3 &a)
{
	Vector3 c;

	c.x = f / a.x;
	c.y = f / a.y;
	c.z = f / a.z;

	return c;
}

inline float Vector3Dot(Vector3 const& a, Vector3 const &b)
{
	float result = a.x * b.x + a.y * b.y + a.z * b.z;
	return result;
}

inline Vector3 Vector3Cross(Vector3 const& a, Vector3 const& b)
{
	Vector3 c;

	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;

	return c;
}

inline Vector3 VectorLerp(Vector3 &a, Vector3 &b, float t)
{
	Vector3 c = a + (b - a) * t;
	return c;
}

inline Vector3 VectorGetMax(Vector3 &a, Vector3 &b)
{
	Vector3 c;

	c.x = a.x > b.x ? a.x : b.x;
	c.y = a.y > b.y ? a.y : b.y;
	c.z = a.z > b.z ? a.z : b.z;

	return c;
}

inline Vector3 VectorGetMin(Vector3 &a, Vector3 &b)
{
	Vector3 c;

	c.x = a.x < b.x ? a.x : b.x;
	c.y = a.y < b.y ? a.y : b.y;
	c.z = a.z < b.z ? a.z : b.z;

	return c;
}

inline Vector3 VectorReciprocal(Vector3 &a)
{
	Vector3 result = a;
	result.Reciprocal();
	return result;
}

inline Vector3 VectorSqrt(Vector3 &a)
{
	Vector3 result = a;
	result.Sqrt();
	return result;
}

inline Vector3 VectorReciprocalSqrt(Vector3 &a)
{
	Vector3 result = a;
	result.ReciprocalSqrt();
	return result;
}

inline Vector3 Vector3Normalize(Vector3 &a)
{
	Vector3 result = a;
	result.Normalize();
	return result;
}

inline float VectorNormalizeAndGetLength(Vector3 &a, Vector3 &result)
{
	result = a;
	float vectorlength = result.NormalizeAndGetLength();
	return vectorlength;
}

inline Vector3 VectorAbs(Vector3 &a)
{
	Vector3 result = a;
	result.Abs();
	return result;
}

inline Vector3 VectorInvertSign(Vector3 &a)
{
	Vector3 result = a;
	result.InvertSign();

	return result;
}

inline Vector3 VectorSaturate(Vector3 &a)
{
	Vector3 result = a;
	result.Saturate();
	return result;
}


inline Vector3 PointToPlaneDistance(Vector3 &point, Vector3 &plane)
{
	return Vector3Dot(point, plane);
}

inline Vector3 MirrorPoint(Vector3 &point, Vector3 &plane)
{
	Vector3 mirrored_point;
	
	float d = PointToPlaneDistance(point, plane)[0];
	mirrored_point = point - (2.0f * d ) * plane;

	return mirrored_point;
}

// 計算vector對plane平面鏡射後的3D方向
inline Vector3 MirrorVector(Vector3 &vector, Vector3 &plane)
{
	Vector3 mirrored_a, mirrored_b, mirrored_vector;
	Vector3 vZero = Vector3::GetZero();

	float d = PointToPlaneDistance(vector, plane)[0];
	mirrored_a = vector - (2.0f * d ) * plane;

	d = plane[3];
	mirrored_b = vZero - (2.0f * d ) * plane;
	
	mirrored_vector = mirrored_a - mirrored_b;
	mirrored_vector[3] = 1.0f;

	return mirrored_vector;
}

#endif // _VECTOR4_SSE_