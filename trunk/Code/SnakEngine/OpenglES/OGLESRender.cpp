#include <OpenglES/OGLESRender.h>

#include <boost/typeof/typeof.hpp>

namespace SE
{
	//
	void OGLESRender::BindBuffer(GLenum eBufType, GLuint iBuf)
	{
		BOOST_AUTO(iter, m_BindedBuffers.find(eBufType));
		if ((iter == m_BindedBuffers.end()) || (iter->second != iBuf))
		{
			glBindBuffer(eBufType, iBuf);
			m_BindedBuffers[eBufType] = iBuf;
		}
	}

    void OGLESRender::DeleteBuffer(GLsizei n, GLuint const * cpBufs)
	{
		for (GLsizei i = 0; i < n; ++ i)
		{
			for (BOOST_AUTO(iter, m_BindedBuffers.begin()); iter != m_BindedBuffers.end();)
			{
				if (iter->second == cpBufs[i])
				{
					//在调用 erase 直线先移动到下一个元素位置
					m_BindedBuffers.erase(iter ++);
				}
				else
				{
					++ iter;
				}
			}
		}
		glDeleteBuffers(n, cpBufs);
	}

	void OGLESRender::StartRendering()
	{}

	void OGLESRender::BeginFrame()
	{}
	void OGLESRender::BeginPass()
	{}
	void OGLESRender::EndPass()
	{}
	void OGLESRender::EndFrame()
	{}
				// Just for debug or profile propose
	void OGLESRender::ForceFlush()
	{}
	bool OGLESRender::GetFullScreen() const
	{
		return m_bFullSc;
	}
	void OGLESRender::SetFullScreen(bool bFull)
	{
		m_bFullSc = bFull;
	}
}


