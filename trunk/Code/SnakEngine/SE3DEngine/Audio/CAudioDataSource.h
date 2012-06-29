#ifndef _CAudioDataSource_H_
#define _CAudioDataSource_H_

#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	enum EAudioFormat
	{
		AF_Mono8,
		AF_Mono16,
		AF_Stereo8,
		AF_Stereo16,

		AF_Unknown,
	};

	class DLL_API CAudioDataSource
	{
	public:
		//static CAudioDataSourcePtr NullObject();

		virtual void Open(CFileDataStream const & cpFile) = 0;
		virtual void Close() = 0;

		EAudioFormat GetFormat() const;
		uint32 GetFreq() const;

		virtual size_t GetSize() = 0;

		virtual size_t Read(void* pData, size_t Size) = 0;
		virtual void Reset() = 0;

		virtual ~CAudioDataSource();

	protected:
		EAudioFormat		m_Format;
		uint32		    m_uFreq;
	};

	class DLL_API CAudioDataSourceFactory
	{
	public:
		virtual ~CAudioDataSourceFactory()
		{
		}

		static CAudioDataSourceFactoryPtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual CAudioDataSourcePtr MakeAudioDataSource() = 0;
	};

}

#endif