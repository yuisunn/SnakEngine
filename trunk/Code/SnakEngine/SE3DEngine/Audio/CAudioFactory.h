#ifndef _CAudioFactory_H_
#define _CAudioFactory_H_

#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	class DLL_API CAudioFactory
	{
	public:
		virtual ~CAudioFactory()
			{ }

		//static CAudioFactoryPtr NullObject();

		virtual std::wstring const & GetName() const = 0;

		CAudio& InstAudio();
		virtual CAudioBufferPtr MakeSoundBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uNumSource = 1) = 0;
		virtual CAudioBufferPtr MakeMusicBuffer(CAudioDataSourcePtr const& cpDataSource, uint32 uBufferSeconds = 2) = 0;

	private:
		virtual CAudioPtr MakeAudio() = 0;

	private:
		CAudioPtr m_pAudio;
	};

	//template <typename AudioEngineType, typename SoundBufferType, typename MusicBufferType>
	//class ConcreteAudioFactory : boost::noncopyable, public AudioFactory
	//{
	//public:
	//	ConcreteAudioFactory(std::wstring const& cpName)
	//		: m_sName(cpName)
	//		{ }

	//	std::wstring const & GetName() const
	//		{ return m_sName; }

	//	AudioBufferPtr MakeSoundBuffer(AudioDataSourcePtr const& cpDataSource, uint32 uNumSource = 1)
	//	{
	//		return MakeSharedPtr<SoundBufferType>(dataSource, numSource,
	//			this->AudioEngineInstance().SoundVolume());
	//	}

	//	AudioBufferPtr MakeMusicBuffer(AudioDataSourcePtr const& cpDataSource, uint32 uBufferSeconds = 2)
	//	{
	//		return MakeSharedPtr<MusicBufferType>(dataSource, bufferSeconds,
	//			this->AudioEngineInstance().MusicVolume());
	//	}

	//private:
	//	AudioEnginePtr MakeAudioEngine()
	//	{
	//		return MakeSharedPtr<AudioEngineType>();
	//	}

	//private:
	//	std::wstring const m_sName;
	//};

}

#endif
