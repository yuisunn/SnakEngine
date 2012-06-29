//对不同类型文件读取接口定义

#ifndef _DataStream_H_
#define _DataStream_H_
#include <SE3DEngine/Comm/Def.h>
namespace SE
{
	//dataStream 基类
	struct IDataStream
	{
	public:
	     enum EAccessMode
		{
			READ = 1, 
			WRITE = 2
		};

		   virtual void Open();
		   virtual uint16 GetAcessMode()const;
		   virtual void Seekg(int64 iOffset, std::ios_base::seekdir way);
		   virtual int64 Tellg();
		   virtual bool IsRead() const;
		   virtual bool IsWrite() const;
		   virtual size_t Read(void* pBuf,size_t uCount)=0;
		   virtual size_t Write(const void* pBuf, size_t uCount)=0;
		   virtual void Clear()=0;
		   virtual void Close();
	};

	

}
#endif
