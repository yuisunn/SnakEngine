#ifndef _RenderFactory_H_
#define _RenderFactory_H_

#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	class RenderFactory
	{
	public:
		static RenderFactory& NullObj()
		{
			static RenderFactoryPtr obj = MakeSharedPtr<RenderFactory>();
		    return obj;
		}
	private:
		RenderFactoryPtr m_ReFac;
	}
}

#endif

