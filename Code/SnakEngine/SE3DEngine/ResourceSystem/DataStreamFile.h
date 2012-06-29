#ifndef _DataStreamFile_H_
#define _DataStreamFile_H_

#include <SE3DEngine/ResourceSystem/IDataStream.h>
#include <SE3DEngine/Comm/Def.h>
namespace SE
{
	class DataStreamFile : public IDataStream
	{
	public:
		DataStreamFile(String sName,std::shared_ptr<std::istream> const &stream):m_ResName(sName),m_pIstream(stream)
		{}
		~DataStreamFile();

		void Open();
		uint16 GetAcessMode()const;
		void Seekg(int64 iOffset, std::ios_base::seekdir way);
		int64 Tellg();
		bool IsRead() const;
		bool IsWrite() const;
		size_t Read(void* pBuf,size_t size);
		size_t Write(const void* pBuf, size_t size);
		void Clear();
		void Close();
	private:
		String m_ResName;
		uint64 timestamp;
		std::shared_ptr<std::istream> m_pIstream;
	};
}
#endif

