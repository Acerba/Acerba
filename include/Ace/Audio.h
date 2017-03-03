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

		AudioClip(const File& file, float volume = 1.0f, bool loop = false, UInt32 priority = 10);

		inline AudioClipImpl* operator->() const
		{
			return impl.get();
		}

		/**
			@brief Sets the volume for specified clip between 0.0f and 1.0f.
		*/
		void SetVolume(float volume);

		/**
			@brief Returns the volume value for specified clip.
			@return Volume
		*/
		float GetVolume();

		/**
			@brief Sets pitch value for specified clip.
		*/
		void SetPitch(float pitch);

		/**
			@brief Returns the pitch value for specified clip.
			@return Pitch
		*/
		float GetPitch();

		/**
			@brief Set specified clip on loop.
		*/
		void SetLoop(bool loop);

		/**
			@brief Sets time elapsed for the specified clip. 
		*/
		void SetElapsedTime(double time);

		/**
			@brief Returns elapsed time for specified clip
			@return Elapsed time.
		*/
		double GetElapsedTime();

		/**
			@brief Returns total time of specified clip
			@return total time.
		*/
		double GetTotalTime();

		/**
			@brief Returns true if the specified clip is playing audio.
			@return Is Playing.
		*/
		bool IsPlaying();

		/**
			@brief Sets the priority of the specified clip.
		*/
		void SetPriority(UInt32);

		/**
			@brief Returns the clip priority.
			@return Priority
		*/
		UInt32 GetPriority();

		//void SetPosition(math::Vector3 position);
		//void SetVelocity(math::Vector3 velocity);

	};

	class Audio
	{
	public:
		/**
			@brief Inits Audio.
		*/
		static void Init();

		/**
			@brief Pauses the specified clip.
		*/
		static void PauseAudio(AudioClip& Clip);

		/**
			@brief Pauses or plays the specified clip.
		*/
		static void PauseAudio(AudioClip& Clip, bool ispaused);

		/**
			@brief Updates the specified clip.
		*/
		static bool Update(const AudioClip& Clip);

		/**
			@brief Plays audio with current set of attribute modifiers.
			@return Is Playing
		*/
		static void PlayAudio(const AudioClip& Clip);

		//static void PlayAudioAtPosition(const AudioClip& Clip, Vector3 pos);
		//static void SetAttributes(vector3 apPos, const float* apVel, const float* apForward, const float* apUpward);

		/**
			@brief Stops the specific clip.
		*/
		static void StopAudio(const AudioClip& Clip);

		/**
			@brief Stops all clips.
		*/
		static void StopAll();

		/**
			@brief Sets the master volume.
		*/
		static void SetMasterVolume(float volume);

	private:
	
	};
	
}