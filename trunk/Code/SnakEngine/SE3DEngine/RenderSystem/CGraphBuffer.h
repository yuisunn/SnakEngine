
// ���� vector index bufer
//��c��ʽ���x

#ifndef _GraphBuffer_H_
#define _GraphBuffer_H_

#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	//buffer�洢����
	enum EBufferUsage
	{
		BU_Static,
		BU_Dynamic
	};
	//buffer ��������
	enum EBufferAccess
	{
		BA_Read_Only,
		BA_Write_Only,
		BA_Read_Write
	};



	class DLL_API CGraphBuffer
	{
	public:
		//2�����ϲ�����ʽת��
		explicit CGraphBuffer(EBufferUsage eUsage, uint32 uAcces);
		virtual ~CGraphBuffer();


		//static GraphBufferPtr NullObject();

		void Resize(uint32 m_uByteSize);
		uint32 GetSize() const
		{
			return m_uByteSize;
		}

		EBufferUsage GetUsage() const
		{
			return m_eUsage;
		}

		uint32 Acces() const
		{
			return m_uAcces;
		}

	    virtual void CopyToBuffer(CGraphBuffer& pRhs);


	    virtual void DoResize();
	    virtual void* Map(EBufferAccess eAcces);
		virtual void Unmap();

	protected:
		EBufferUsage m_eUsage;
		uint32 m_uAcces;

		uint32  m_uByteSize;
	};

	class BufferStream
	{

		CGraphBuffer& m_pGbuf;
		void* m_pData;
	};

	//��c��
	struct SVertexBuffer
	{
 
	 // SVertexBuffer()
	 // {
		//m_eVF = VF_Unknown;
		//m_nVerts = 0;
	 // }
	 // SVertexBuffer(void* pData, EVertexFormat eVF, int nVerts=0)
	 // {
		//m_VS.m_pLocalData = pData;
		//m_eVF = eVF;
		//m_nVerts = nVerts;
	 // }
	  //SBufferStream m_VS;
	  //EVertexFormat m_eVF;
	  uint32 m_nVerts;
	};
	//�sӰ��
	struct SIndexBuffer
	{
 
	 // SIndexBuffer()
	 // {
		//m_nInds = 0;
	 // }

	 // SIndexBuffer(uint16* pData)
	 // {
		//m_VS.m_pLocalData = pData;
		//m_nInds = 0;
	 // }
	 // SBufferStream m_VS;
	 // uint32 m_nInds;
	};
}
#endif

