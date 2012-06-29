#ifndef _DataStreamXml_H_
#define _DataStreamXml_H_
#include <SE3DEngine/ResourceSystem/IDataStream.h>


namespace SE
{
		class DataStreamFile : public IDataStream
		{
			   virtual void Seekg(int64 offset, std::ios_base::seekdir way);
			   virtual int64 Tellg();

		};
	
		class DataStreamXml : public IDataStream
		{
				void Open();
				void Seekg(int64 iOffset, std::ios_base::seekdir way);
				int64 Tellg();	
				void Clear();
				operator void*() const;
				bool operator!() const;

		   private:
				uint64 m_uTimestamp;
				boost::shared_ptr<std::istream> m_pIstream;
		};
}

#endif