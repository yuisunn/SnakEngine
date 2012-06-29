#include <SE3DEngine/RenderSystem/CGraphBuffer.h>
#include <boost/bind.hpp>

namespace SE
{
	CGraphBuffer::CGraphBuffer(EBufferUsage eUsage, uint32 uAcces)
		: m_eUsage(eUsage), m_uAcces(uAcces),m_uByteSize(0)
	{
	}

	CGraphBuffer::~CGraphBuffer()
	{
	}

	//CGraphBufferPtr CGraphBuffer::NullObject()
	//{
	//	static CGraphBufferPtr obj = MakeSharedPtr<CGraphBuffer>();
	//	return obj;
	//}

	void CGraphBuffer::Resize(uint32 uByteSize)
	{
		m_uByteSize = uByteSize;
		this->DoResize();
	}
}