
#ifndef _Util_H_
#define _Util_H_


#include <boost/mem_fn.hpp>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <cwchar>
#include <SE3DEngine/Config/Platform.h>

#define UNREF_PARAM(x) (x)

//指针如果一场就delete 释放
namespace SE
{
	// 得到COM对象的智能指针
	template <typename T>
	inline boost::shared_ptr<T>
	MakeCOMPtr(T* p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}

	template <typename T>
	inline boost::shared_ptr<T> MakeSharedPtr()
	{
		return boost::shared_ptr<T>(new T, boost::checked_deleter<T>());
	}

	template <typename T, typename A1>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1)
	{
		return boost::shared_ptr<T>(new T(a1), boost::checked_deleter<T>());
	}

	template <typename T, typename A1>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1)
	{
		return boost::shared_ptr<T>(new T(a1), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2)
	{
		return boost::shared_ptr<T>(new T(a1, a2), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2)
	{
		return boost::shared_ptr<T>(new T(a1, a2), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2, A3 const & a3)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline boost::shared_ptr<T> MakeSharedPtr(A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7, A8 const & a8)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7, A8& a8)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7, A8& a8, A9& a9)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7, A8& a8, A9& a9, A10& a10)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7, A8& a8, A9& a9, A10& a10, A11& a11)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11), boost::checked_deleter<T>());
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline boost::shared_ptr<T> MakeSharedPtr(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7, A8& a8, A9& a9, A10& a10, A11& a11, A12& a12)
	{
		return boost::shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12), boost::checked_deleter<T>());
	}
}
//需要特别说明的是,以上假设机器是每个内存单元以8位即一个字节为单位的. 
// 简单的说,ittle endian把低字节存放在内存的低位；而big endian将低字节存放在内存的高位. 
//现在主流的CPU,intel系列的是采用的little endian的格式存放数据,而motorola系列的CPU采用的是big endian. 
namespace SE
{
		// 把一个wstring转化为string
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, std::wstring const & src)
	{
#ifdef SE_WINDOWS
		int const mbs_len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), NULL, 0, NULL, NULL);
		std::vector<char> tmp(mbs_len + 1);
		WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], mbs_len, NULL, NULL);
#else
		std::setlocale(LC_CTYPE, "");

		size_t const mbs_len = wcstombs(NULL, src.c_str(), 0);
		std::vector<char> tmp(mbs_len + 1);
		wcstombs(&tmp[0], src.c_str(), tmp.size());
#endif

		dest.assign(tmp.begin(), tmp.end() - 1);

		return dest;
	}
		// 把一个string转化为wstring
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring& Convert(std::wstring& dest, std::string const & src)
	{
#ifdef SE_WINDOWS
		int const wcs_len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), NULL, 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], wcs_len);
#else
		std::setlocale(LC_CTYPE, "");

		size_t const wcs_len = mbstowcs(NULL, src.c_str(), 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		mbstowcs(&tmp[0], src.c_str(), src.size());
#endif

		dest.assign(tmp.begin(), tmp.end() - 1);

		return dest;
	}

 

	template <int size>
	void NativeToBigEndian(void* p)
	{
	#ifdef SE_LITTLE_ENDIAN
		EndianSwitch<size>(p);
	#else
		UNREF_PARAM(p);
	#endif
	}
	template <int size>
	void NativeToLittleEndian(void* p)
	{
	#ifdef SE_LITTLE_ENDIAN
		UNREF_PARAM(p);
	#else
		EndianSwitch<size>(p);
	#endif
	}

	template <int size>
	void BigEndianToNative(void* p)
	{
		NativeToBigEndian<size>(p);
	}
	template <int size>
	void LittleEndianToNative(void* p)
	{
		NativeToLittleEndian<size>(p);
	}

		// 产生FourCC常量
	template <unsigned char ch0, unsigned char ch1, unsigned char ch2, unsigned char ch3>
	struct MakeFourCC
	{
		enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) };
	};
	//应该是通过静态断言来判断是否是子类，省去dynamic_cast的消耗。 
	template <typename To, typename From>
	inline To checked_cast(From p)
	{
		BOOST_ASSERT(dynamic_cast<To>(p) == static_cast<To>(p));
		return static_cast<To>(p);
	}
		//如果动态转换盒静态一样 那么静态转换  可提高效率
	template <typename To, typename From>
	inline boost::shared_ptr<To> checked_pointer_cast(boost::shared_ptr<From> const & p)
	{
		BOOST_ASSERT(boost::dynamic_pointer_cast<To>(p) == boost::static_pointer_cast<To>(p));
		return boost::static_pointer_cast<To>(p);
	}
	#ifdef SELECT1ST2ND_SUPPORT
	template <typename pair_type>
	struct select1st : public std::unary_function<pair_type, typename pair_type::first_type>
	{
		typename pair_type::first_type const & operator()(pair_type const & x) const
		{
			return x.first;
		}
	};

	template <typename pair_type>
	struct select2nd : public std::unary_function<pair_type, typename pair_type::second_type>
	{
		typename pair_type::second_type const & operator()(pair_type const & x) const
		{
			return x.second;
		}
	};
#else
	using std::select1st;
	using std::select2nd;
#endif		// _SELECT1ST2ND_SUPPORT

}

#endif