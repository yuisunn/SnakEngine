#ifndef _DataOgg_
#define _DataOgg_

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <istream>
#include <SE3DEngine/Audio/CAudioDataSource.h>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/ResourceSystem/CDataStream.h>

namespace SE
{
	class OggDataSource : public CAudioDataSource
	{
	public:
		OggDataSource();
		~OggDataSource();
		 
		void Open(CFileDataStream const& cpFile);
		void Close();

		size_t GetSize();
		size_t Read(void* pData, size_t uSize);
		void Reset();
	private:
		static size_t VorbisRead(void* pPtr, size_t uByteSize, size_t uSizeToRead, void* pDataSource);
		static int VorbisSeek(void* pDataSource, ogg_int64_t iOffset, int iWhence);
		static int VorbisClose(void* pDataSource);
		static long VorbisTell(void* pDataSource);
	private:
		CFileDataStreamPtr m_pOggFile;
		int64 m_iLength;
		OggVorbis_File m_OggV_File;
	};
}
#endif
