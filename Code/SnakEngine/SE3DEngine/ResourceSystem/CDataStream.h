//数据操作基类

#ifndef _DataStream_H_
#define _DataStream_H_

#include <istream>

#include <SE3DEngine/Comm/Def.h>


namespace SE
{
	enum EAccessMode
	{
		AM_READ = 1, 
		AM_WRITE = 2
	};
		//dataStream 基类
	class CFileDataStream
	{
	public:

		CFileDataStream(String const& sName, uint16 eAcces = AM_READ);
	    virtual ~CFileDataStream();

		virtual void Open();
		String const& GetName() const;
		void SetName(String const& cpName);
		uint16 GetAccessMode()const;
		void SetAccessMode(uint16 const& eAcces);
		virtual void Seekg(int64 iOffset, std::ios_base::seekdir way);
		virtual int64 Tellg();
		virtual void Read(void* pBuf,size_t uSize);
		virtual size_t Write(const void* pBuf, size_t uCount);
		int64 GetCount() const;
		virtual void Clear();
		virtual void Close();
		operator void*() const
		{
			return m_pIstream->operator void*();
		}
		bool operator!() const
		{
			return m_pIstream->operator!();
		}
	private:
		String m_sName;
		uint16 m_eAcces;
		size_t m_uSize;
		boost::shared_ptr<std::istream> m_pIstream;
	};

	

}
#endif
