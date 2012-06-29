#include <OpenglES/OGLESGraphBuffer.h>

#include <boost/assert.hpp>

#include <SE3DEngine/Comm/Util.h>

#include <SE3DEngine/Comm/SEContext.h>
#include <OpenglES/OGLESRender.h>


//是用 vector 是否效率低下
namespace SE
{
	OGLESGraphBuffer::OGLESGraphBuffer(EBufferUsage eUsage, uint32 uAcces, GLenum eBufType, PixelInitData const* cpInitData)
		:CGraphBuffer(eUsage,uAcces),m_eBufType(eBufType)
	{
		BOOST_ASSERT((GL_ARRAY_BUFFER == eBufType) || (GL_ELEMENT_ARRAY_BUFFER == eBufType));
		glGenBuffers(1,&m_buf);
		if (cpInitData != NULL)
		{
			m_uByteSize = cpInitData->m_uRowPitch;

			OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());
			re.BindBuffer(m_eBufType, m_buf);
			glBufferData(m_eBufType,
				static_cast<GLsizeiptr>(m_uByteSize), cpInitData->m_cpData,
					(BU_Static == m_eUsage) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
			m_uDatas.assign(static_cast<uint8 const *>(cpInitData->m_cpData),
				static_cast<uint8 const *>(cpInitData->m_cpData) + m_uByteSize);
		}
	}
	OGLESGraphBuffer::~OGLESGraphBuffer()
	{
		if(SEContext::Instance().FactoryIsNull())
		{
			OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());
			re.DeleteBuffer(1,&m_buf);

		}
		else
		{
			glDeleteBuffers(1,&m_buf);

		}
	}
	void OGLESGraphBuffer::Active()
	{
		glBindBuffer(m_eBufType,m_buf);
	}
	void* OGLESGraphBuffer::Map(EBufferAccess eAcces)
	{
		m_eAcces = eAcces;

		switch (eAcces)
		{
		case BA_Write_Only:
			// TODO: fix OES_mapbuffer
			/*if (glloader_GLES_OES_mapbuffer())
			{
				OGLES2RenderEngine& re = *checked_cast<OGLES2RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
				re.BindBuffer(target_, vb_);
				return glMapBufferOES(target_, GL_WRITE_ONLY_OES);
			}
			else*/
			{
				return &m_uDatas[0];
			}

		default:
			return &m_uDatas[0];
		}
	}
	void OGLESGraphBuffer::Unmap()
	{
		switch (m_eAcces)
		{
		case BA_Write_Only:
		case BA_Read_Write:
			OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());
			re.BindBuffer(m_eBufType, m_buf);
			// TODO: fix OES_mapbuffer
			/*if (glloader_GLES_OES_mapbuffer())
			{
				glUnmapBufferOES(target_);
			}
			else*/
			{
				glBufferSubData(m_eBufType, 0, static_cast<GLsizeiptr>(m_uByteSize), &m_uDatas[0]);
			}
			break;
		}
	}
	void OGLESGraphBuffer::DoResize()
	{
		BOOST_ASSERT(m_uByteSize != 0);

		OGLESRender& re = *checked_cast<OGLESRender*>(&SEContext::Instance().InstFactory().InstRender());
		re.BindBuffer(m_eBufType,m_buf);
		glBufferData(m_eBufType,static_cast<GLsizeiptr>(m_uByteSize),NULL,(BU_Static==m_eUsage)?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
		m_uDatas.resize(m_uByteSize);

	}
	//void OGLESGraphBuffer::CopyToBuffer(OGLESGraphBuffer& pRhs)
	//{
	//	GraphBuffer::Mapper lhs_mapper(*this, BA_Read_Only);
	//	GraphBuffer::Mapper rhs_mapper(rhs, BA_Write_Only);
	//	std::copy(lhs_mapper.Pointer<uint8_t>(), lhs_mapper.Pointer<uint8_t>() + size_in_byte_,
	//		rhs_mapper.Pointer<uint8_t>());
	//}



}