#include <SE3DEngine/ResourceSystem/OggDataSource.h>

#include <SE3DEngine/Comm/ThrowErr.h>

namespace SE
{
	// 构造函数
	/////////////////////////////////////////////////////////////////////////////////
	OggDataSource::OggDataSource()
	{
	}

	// 析构函数
	/////////////////////////////////////////////////////////////////////////////////
	OggDataSource::~OggDataSource()
	{
		ov_clear(&m_OggV_File);
	}

	void OggDataSource::Open(CFileDataStream const& cpFile)
	{
		//m_pOggFile = cpFile;

		m_pOggFile->Seekg(0, std::ios_base::end);
		m_iLength = m_pOggFile->Tellg();
		m_pOggFile->Seekg(0, std::ios_base::beg);

		//这个接口里定义有ogg 内部么哦人操作函数
		ov_callbacks vorbis_callbacks;
		vorbis_callbacks.read_func = OggDataSource::VorbisRead;
		vorbis_callbacks.close_func = OggDataSource::VorbisClose;
		vorbis_callbacks.seek_func = OggDataSource::VorbisSeek;
		vorbis_callbacks.tell_func = OggDataSource::VorbisTell;
		//断言
		Verify(0 == ov_open_callbacks(this, &m_OggV_File, NULL, 0, vorbis_callbacks));
		//这个结构定义 ogg的基本信息
		vorbis_info* vorbis_info = ov_info(&m_OggV_File, -1);
		m_Format = (1 == vorbis_info->channels) ? AF_Mono16 : AF_Stereo16;
		m_uFreq = vorbis_info->rate;

		this->Reset();
	}

	void OggDataSource::Close()
	{
		m_pOggFile.reset();
		this->Reset();
	}

	// 读取Ogg数据
	/////////////////////////////////////////////////////////////////////////////////
	size_t OggDataSource::Read(void* pData, size_t uSize)
	{
		BOOST_ASSERT(pData != NULL);

		char* pcm = static_cast<char*>(pData);

		size_t cur_size = 0;
		int section;
		while (cur_size < uSize)
		{
			//pcm 解码后的地址指针 参数3 宽度 4 Specifies big or little endian byte packing. 0 for little endian, 1 for b ig endian. Typical value is 0.
			//5 Specifies word size 6 Signed or unsigned data  最后一个是 数据流指针
			int result = ov_read(&m_OggV_File, pcm + cur_size, static_cast<int>(uSize - cur_size), 0, 2, 1, &section);

			if (result > 0)
			{
				cur_size += result;
			}
			else
			{
				if (result <= 0)
				{
					break;
				}
			}
		}

		return cur_size;
	}

	// 返回数据源大小
	/////////////////////////////////////////////////////////////////////////////////
	size_t OggDataSource::GetSize()
	{
		vorbis_info* vorbis_info = ov_info(&m_OggV_File, -1);
		return static_cast<size_t>(ov_pcm_total(&m_OggV_File, -1) * vorbis_info->channels * sizeof(ogg_int16_t));
	}

	// 数据源复位
	/////////////////////////////////////////////////////////////////////////////////
	void OggDataSource::Reset()
	{
		m_pOggFile->Clear();
		ov_pcm_seek(&m_OggV_File, 0);
	}

	size_t OggDataSource::VorbisRead(void* pPtr, size_t uByteSize, size_t uSizeToRead, void* pDataSource)
	{
		// Get the data in the right format
		OggDataSource* vorbis_data = static_cast<OggDataSource*>(pDataSource);

		size_t actual_size_to_read;	// How much data we are actually going to read from memory
		// Calculate how much we need to read.  This can be sizeToRead*byteSize or less depending on how near the EOF marker we are
		size_t space_to_eof = static_cast<size_t>(vorbis_data->m_iLength - vorbis_data->m_pOggFile->Tellg());
		if (uSizeToRead * uByteSize < space_to_eof)
		{
			actual_size_to_read = uSizeToRead * uByteSize;
		}
		else
		{
			actual_size_to_read = space_to_eof;
		}

		// A simple copy of the data from memory to the datastruct that the vorbis libs will use
		if (actual_size_to_read > 0)
		{
			vorbis_data->m_pOggFile->Read(pPtr, static_cast<std::streamsize>(actual_size_to_read));
		}

		// Return how much we read (in the same way fread would)
		return actual_size_to_read;
	}

	int OggDataSource::VorbisSeek(void* pDataSource, ogg_int64_t iOffset, int iWhence)
	{
		OggDataSource* vorbis_data = static_cast<OggDataSource*>(pDataSource);

		std::ios_base::seekdir dir;
		switch (iWhence)
		{
		case SEEK_SET: // Seek to the start of the data file
			dir = std::ios_base::beg;
			break;

		case SEEK_CUR: // Seek from where we are
			dir = std::ios_base::cur;
			break;

		case SEEK_END: // Seek from the end of the file
			dir = std::ios_base::end;
			break;

		default:
			BOOST_ASSERT(false);
			dir = std::ios_base::beg;
			break;
		};

		vorbis_data->m_pOggFile->Seekg(static_cast<long>(iOffset), dir);

		return 0;
	}

	int OggDataSource::VorbisClose(void* pDataSource)
	{
		OggDataSource* vorbis_data = static_cast<OggDataSource*>(pDataSource);
		vorbis_data->m_pOggFile.reset();
		return 1;
	}

	long OggDataSource::VorbisTell(void* pDataSource)
	{
		OggDataSource* vorbis_data = static_cast<OggDataSource*>(pDataSource);
		return static_cast<long>(vorbis_data->m_pOggFile->Tellg());
	}

}
