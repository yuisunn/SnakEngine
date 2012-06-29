#ifndef _ThrowErr_H_
#define _ThrowErr_H_

#include <string>
#include <stdexcept>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif

namespace SE
{
	 std::string CombineFileLine(std::string const & file, int line);
}

#define THR(x)			{ throw boost::system::system_error(boost::system::posix_error::make_error_code(x), SE::CombineFileLine(__FILE__, __LINE__)); }

// 如果错误，就抛出错误代码
#define ErO(x)			{ HRESULT _hr = x; if (static_cast<HRESULT>(_hr) < 0) { throw std::runtime_error(SE::CombineFileLine(__FILE__, __LINE__)); } }

// 断言
inline void
Verify(bool x)
{
	if (!x)
	{
		THR(boost::system::posix_error::not_supported);
	}
}

#endif