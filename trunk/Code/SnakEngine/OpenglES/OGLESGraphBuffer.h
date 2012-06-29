#ifndef _OGLESGraphBuffer_H_
#define _OGLESGraphBuffer_H_

#include <GLES2/gl2.h>

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/PixelFormat.h>
#include <SE3DEngine/RenderSystem/CGraphBuffer.h>


namespace SE
{
	class OGLESGraphBuffer : public CGraphBuffer
	{
	public:
		explicit OGLESGraphBuffer(EBufferUsage eUsage, uint32 uAcces, GLenum eBufType, PixelInitData const* cpInitData);
		~OGLESGraphBuffer();

		void CopyToBuffer(OGLESGraphBuffer& pRhs);
		void Active();
		GLuint GLvbo() const
		{
			return m_buf;
		}
		GLuint GLType() const
		{
			return m_eBufType;
		}
			private:
		void DoResize();
		void* Map(EBufferAccess eAcces);
		void Unmap();

	private:
		GLuint m_buf;
		GLenum m_eBufType;
		
		EBufferAccess m_eAcces;
		std::vector<uint8> m_uDatas;
		
	};


}

#endif
