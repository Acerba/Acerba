#pragma once

#include <Ace/File.h>


#include <memory>

namespace ace
{
	class AudioSample
	{
	public:
		struct AudioSampleImpl;

	private:
		std::shared_ptr<AudioSampleImpl> impl;

	public:

		AudioSample(AudioSampleImpl* sample = nullptr) : impl(sample)
		{

		}

		AudioSample(const File& file);
	

		inline AudioSampleImpl* operator->() const
		{
			return impl.get();
		}

	};

	class Audio
	{
	public:
		static void Init();
		static bool Update(const AudioSample& sample);
		static void PlayAudio(const AudioSample& sample);

	private:
	
	};
	
}