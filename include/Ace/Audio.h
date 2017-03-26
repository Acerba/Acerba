#pragma once

#include <Ace/Vector3.h>
#include <Ace/File.h>
#include <Ace/Math.h>

#include <Ace/Macros.h>

#include <vector>
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


		AudioClip(AudioClipImpl* clip = nullptr);

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

		/**
			@brief Sets  audio position for the specified source
			@param[in] Position
		*/
		void SetPosition(math::Vector3 position);
		
		/**
			@brief Sets  audio position for the specified source
			@param[in] Velocity
		*/
		void SetVelocity(math::Vector3 velocity);

		/**
			@brief fades audio
			@param[in] duration, endvolume
		*/
		void Fade(float duration, float endVolume);

	};

	class IAudioEffect
	{
	public:

		float duration;
		float time;
		AudioClip clip;

		IAudioEffect(const AudioClip& clip, float duration) : clip(clip), duration(duration), time(0)
		{

		}

		virtual ~IAudioEffect()
		{

		}

		virtual void Effect(float normalizedTime) = 0;


	};

	class FadeEffect : public IAudioEffect
	{
	public:
		float volume;
		float endVolume;

		FadeEffect(const AudioClip& clip, float duration, float volume, float endVolume) : IAudioEffect(clip, duration), volume(volume), endVolume(endVolume)
		{

		}

		virtual void Effect(float normalizedTime)
		{
			clip.SetVolume(math::Lerp(volume, endVolume, normalizedTime));
		}
	};

	class Audio
	{
	public:
		/**
			@brief Inits Audio.
		*/
		static void Init();

		/**
			@brief Quits Audio.
		*/
		static void Quit();

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
			@param[in] AudioClip&
		*/
		static bool Update(const AudioClip& Clip);

		/**
			@brief Updates the all clips.
		*/
		static void Update();

		/**
			@brief Plays audio with current set of attribute modifiers.
			@param[in] AudioClip&
			@return Is Playing
		*/
		static void PlayAudio(const AudioClip& Clip);

		/**
			@brief Sets 3D audio position for the specified source.
			@param[in] AudioClip&, Vector3 pos
		*/
		static void PlayAudioAtPosition(const AudioClip& Clip, math::Vector3 pos);
		
		/**
			@briefSets 3D audio attributes for the listener.
			@param[in] Vector3 apPos, Vector3 apVel, Vector3 apforward, Vector3 apupward
		*/
		static void SetAttributes(math::Vector3 apPos, math::Vector3 apVel, math::Vector3 apForward, math::Vector3 apUpward);

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
			@param[in] volume
		*/
		static void SetMasterVolume(float volume);

		/**
			@brief Adds effect to array
			@param[in] IAudioEffect
		*/
		static void AddEffect(IAudioEffect* effect);
		

	private:
		
		static Audio& GetAudio();

		ACE_DISABLE_COPY(Audio)

		Audio();
		~Audio();

		std::vector<AudioClip> clips;
		std::vector<IAudioEffect*> effects;
	
	};
	

}