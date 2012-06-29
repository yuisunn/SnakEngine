#ifndef _DataOgg_
#define _DataOgg_

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <istream>

#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/ResourceSystem/CDataStream.h>

namespace SE
{
	class DataAOgg : public CFileDataStream
	{
	public:
		 
		void Open(CDataStream const& pStream);
		void Close();

		size_t Size();
		size_t Read(void* pData, size_t uSize);
		void Reset();
	private:
		static size_t VorbisRead(void* pPtr, size_t uByteSize, size_t uSizeToRead, void* pDataSource);
		static int VorbisSeek(void* pDataSource, ogg_int64_t iOffset, int iWhence);
		static int VorbisClose(void* pDataSource);
		static long VorbisTell(void* pDataSource);

	};
}
#endif
