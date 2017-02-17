#pragma once

#include <Ace/Vector3.h>
#include <Ace/File.h>



#include <memory>

namespace ace
{
	class AudioClip
	{
		friend class Audio;
	public:
		struct AudioClipImpl;


	private:
		std::shared_ptr<AudioClipImpl> impl;
		bool ispaused;

		float volume;
		float pitch;
		UInt32 priority;
		bool loop;
		

	public:

		
		AudioClip(AudioClipImpl* clip = nullptr) : impl(clip)
		{

		}

		AudioClip(const File& file, float volume = 1.0f, bool loop = false, UInt32 priority = 10) : volume(volume), loop(loop), priority(priority)
		{

		}
	

		inline AudioClipImpl* operator->() const
		{
			return impl.get();
		}

		void SetVolume(float volume);
		float GetVolume();
		void SetPitch(float pitch);
		float GetPitch();
		void SetLoop(bool loop);
		void SetElapsedTime(double time);
		double GetElapsedTime();
		double GetTotalTime();
		bool IsPlaying();
		void SetPriority(UInt32);
		UInt32 GetPriority();


		//void SetPosition(math::Vector3 position);
		//void SetVelocity(math::Vector3 velocity);

	};

	class Audio
	{
	public:

		static void Init();
		static void PauseAudio(AudioClip& Clip);
		static void PauseAudio(AudioClip& Clip, bool ispaused);
		static bool Update(const AudioClip& Clip);
		static void PlayAudio(const AudioClip& Clip);
		//static void PlayAudioAtPosition(const AudioClip& Clip, Vector3 pos);
		//static void SetAttributes(vector3 apPos, const float* apVel, const float* apForward, const float* apUpward);
		static void StopAudio(const AudioClip& Clip);
		static void StopAll();
		static void SetMasterVolume(float volume);

	private:
	
	};
	
}