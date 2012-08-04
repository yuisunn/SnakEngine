#ifndef _SEMath_
#define _SEMath_

#define MATH_PI 3.141592653f

#include <limits>
//#include <boost/static_assert.hpp>
#include <SE3DEngine/Config/Config.h>

namespace SEMath
{
	//基本数学运算
	//////////////////////////
#ifdef SE_CPU_X86
	template<class T> inline void Swap(T &a, T &b)
	{

		__asm
		{
			mov eax, dword ptr[a];
			mov ebx, dword ptr[b];
			fld dword ptr[eax]
			fld dword ptr[ebx]
			fstp dword ptr[eax]
			fstp dword ptr[ebx]
		}
	};

	
	inline float Abs(float v)
	{
		float result;
		__asm
		{
			fld [v]
			fabs
			fstp [result]
		}
		return result;
	}

	inline float Sqrt(float v)
	{
		float result;
		__asm
		{
			fld [v]
			fsqrt
			fstp [result]
		}
		return result;
	}

	inline float ReciprocalSqrt(float v)
	{
		float result;
		__asm
		{
			fld [v]
			fsqrt
			fld1
			fdivrp st(1),st
			fstp [result]
		}
		return result;
	}

	inline float Sin(float radian)
	{
		float result;
		__asm
		{
			fld [radian]
			fsin
			fstp [result]
		}
		return result;
	}

	// -pi < p_Angle < pi
	inline float FastSin(float p_Angle)
	{
		const float Factor1 = 4.0f / MATH_PI;
		const float Factor2 = 4.0f / (MATH_PI * MATH_PI);
		return (Factor1 * p_Angle) - (Factor2 * p_Angle * fabs(p_Angle));
	}

	inline float ASin(float v)
	{
		float result = (float) asin(v);
		return result;
	}

	inline float Cos(float radian)
	{
		float result;
		__asm
		{
			fld [radian]
			fcos
			fstp [result]
		}
		return result;
	}

	inline float ACos(float v)
	{
		float result = (float) acos(v);
		return result;
	}

	inline void SinCos(float radian, float &sinvalue, float &cosvalue)
	{
		unsigned short cwd_flag = 0;
		
		__asm
		{
			fstsw [cwd_flag]
		}

		int stacktop = (cwd_flag & 0x3800)>>11;

		if ( stacktop < 7 )
		{
			__asm
			{
				mov eax, dword ptr[cosvalue]
				mov ebx, dword ptr[sinvalue]
				fld [radian]
				fsincos
				fstp dword ptr [eax]
				fstp dword ptr [ebx]
			}
		}
		else
		{
			float s0, s1;
			__asm
			{
				fstp [s0]
				fstp [s1]
				mov eax, dword ptr[cosvalue]
				mov ebx, dword ptr[sinvalue]
				fld [radian]
				fsincos
				fstp dword ptr [eax]
				fstp dword ptr [ebx]
				fld [s1]
				fld [s0]
			}
		}
	}

	inline float Tan(float radian)
	{
		float result;

		unsigned short cwd_flag = 0;

		__asm
		{
			fstsw [cwd_flag]
		}

		int stacktop = (cwd_flag & 0x3800)>>11;
		if ( stacktop < 7 )
		{
			__asm
			{
				fld [radian]
				fptan
				fstp [result]
				fstp [result]
			}
		}
		else
		{
			float s0,s1;
			__asm
			{
				fstp [s0]
				fstp [s1]
				fld [radian]
				fptan
				fstp [result]
				fstp [result]
				fld [s1]
				fld [s0]
			}
		}

		return result;
	}

	inline float Cot(float radian)
	{
		float result;

		unsigned short cwd_flag = 0;
		__asm
		{
			fstsw [cwd_flag]
		}
		int stacktop = (cwd_flag & 0x3800)>>11;
		if ( stacktop < 7 )
		{
			__asm
			{
				fld [radian]
				fptan
				fdivrp st(1), st(0)
				fstp [result]
			}
		}
		else
		{
			float s0,s1;
			__asm
			{
				fstp [s0]
				fstp [s1]
				fld [radian]
				fptan
				fdivrp st(1), st(0)
				fstp [result]
				fld [s1]
				fld [s0]
			}
		}

		return result;
	}

	inline float ATan(float radian)
	{
		float result;
		__asm
		{
			fld [radian]
			fpatan
			fstp [result]
		}
		return result;
	}

	inline float Lerp(float a, float b, float t)
	{
		return a + (b-a) * t;
	}

	inline float frac(float a)
	{
		return a - (int)a;
	}
//#endif
//#ifdef SE_CPU_ARM
#else
	template<class T> inline void Swap(T &a, T &b)
	{

	      T c; c=a; a=b; b=c;  
	};
	inline float Abs(float v)
	{
			union FNI
			{
				float f;
				int32_t i;
			} fni;
			fni.f = v;
			fni.i &= 0x7FFFFFFF;
			return fni.f;
	}

	inline float Sqrt(float v)
	{
		return std::sqrt(v);
	}

	inline float ReciprocalSqrt(float v)
	{
            float const threehalfs = 1.5f;

			float x2 = v * 0.5f;
			union FNI
			{
				float f;
				int32_t i;
			} fni;
			fni.f = v;											// evil floating point bit level hacking
			fni.i = 0x5f375a86 - (fni.i >> 1);						// what the fuck?
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));	// 1st iteration
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));		// 2nd iteration, this can be removed

			return fni.f;
	}

	inline float Sin(float radian)
	{
		return std::sin(radian);
	}

	// -pi < p_Angle < pi
	inline float FastSin(float p_Angle)
	{
		const float Factor1 = 4.0f / MATH_PI;
		const float Factor2 = 4.0f / (MATH_PI * MATH_PI);
		return (Factor1 * p_Angle) - (Factor2 * p_Angle * fabs(p_Angle));
	}

	inline float ASin(float v)
	{
		return std::asin(v);
	}

	inline float Cos(float radian)
	{
		return sin(x + MATH_PI / 2);
	}

	inline float ACos(float v)
	{
		return std::acos(v);
	}

	inline void SinCos(float radian, float &sinvalue, float &cosvalue)
	{
	        sinvalue = sin(radian);
			cosvalue = cos(radian);
	}

	inline float Tan(float radian)
	{
		return std::tan(radian);
	}

	inline float Cot(float radian)
	{
		 return(1 / tan(radian));
	}

	inline float ATan(float radian)
	{
		return std::atan(radian);
	}

	inline float Lerp(float a, float b, float t)
	{
		return a + (b-a) * t;
	}

	inline float frac(float a)
	{
		return a - (int)a;
	}
#endif
    // 判断两个数是否相等
	////////////////////
	template <typename T>
	inline bool
	equal(T const & lhs, T const & rhs)
	{
		return (lhs == rhs);
	}
	///?template <> 函数模板特化：当函数模板需要对某些类型进行特别处理，称为函数模板的特化。 必须有一个普通模板版本实现
	// 浮点版本
	template <>
	inline bool
	equal<float>(float const & lhs, float const & rhs)
	{
		return (abs(lhs - rhs)
			<= std::numeric_limits<float>::epsilon());
	}
	//双字节版
	template <>
	inline bool
	equal<double>(double const & lhs, double const & rhs)
	{
		return (abs(lhs - rhs)
			<= std::numeric_limits<double>::epsilon());
	}
	//平面方程算点面相交
	////////////////////////////
	template<typename T> 
	inline float dot_coord(T const &lhs, T const &rhs)
	{
		return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w;
	}
	inline float DegreeToRadian(float r)
	{
		float radian = r * (MATH_PI / 180.0f);
		return radian;
	}

	inline float DegToRad(float r)
	{
		return DegreeToRadian(r);
	}

	inline float RadianToDegree(float r)
	{
		float degree = r * (180.0f / MATH_PI);
		return degree;
	}

	inline float RadToDeg(float r)
	{
		return RadianToDegree(r);
	}

};



#endif // _SEMath_WIN32_