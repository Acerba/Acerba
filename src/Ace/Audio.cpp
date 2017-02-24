#include <iostream>
#include <string>
#include <Ace/Audio.h>
#include <Ace/IntTypes.h>

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>


namespace ace
{
	struct AudioClip::AudioClipImpl
	{
		cOAL_Sample* clip;
		int id;

		AudioClipImpl(cOAL_Sample* clip) : clip(clip)
		{

		}
	};

	AudioClip::AudioClip(const File& file, float volume, bool loop, UInt32 priority)
	{
		UInt32 pos = file.Size();
		UInt8* buffer = new UInt8[pos];

		file.Read(buffer, pos);
		impl.reset(new AudioClip::AudioClipImpl(OAL_Sample_LoadFromBuffer(buffer, pos)));

		delete buffer;
	}


	void Audio::Init()
	{
		cOAL_Init_Params oal_parms;
		if (OAL_Init(oal_parms) == false)
		{
			printf("Audio initializing failed!\n");
		}
		else
		{
			printf("Audio initializing succeeded!\n");
		}
	}

	bool Audio::Update(const AudioClip& clip)
	{
		return OAL_Source_IsPlaying(clip->id);
	}

	void Audio::PlayAudio(const AudioClip& clip)
	{
		if (clip->clip)
		{
			printf("Channels : %d\nFrequency : %d", clip->clip->GetChannels(), clip->clip->GetFrequency());
			clip->id = OAL_Sample_Play(OAL_FREE, clip->clip, clip.volume, clip.loop, clip.priority);
			OAL_Source_SetPaused(clip->id, false);
		}
		else
		{
			printf("Error!\n");
		}
	}

	void Audio::PauseAudio(AudioClip& clip)
	{
		clip.ispaused = !clip.ispaused;
		PauseAudio(clip, clip.ispaused);
	}

	void Audio::PauseAudio(AudioClip& clip, bool pause)
	{
		OAL_Source_SetPaused(clip->id, pause);
	}

	void Audio::StopAudio(const AudioClip& clip)
	{
		OAL_Source_Stop(clip->id);
	}

	void Audio::StopAll()
	{
		OAL_Source_Stop(OAL_ALL);
	}

	void Audio::SetMasterVolume(float volume)
	{
		OAL_Listener_SetMasterVolume(volume);
	}

	void AudioClip::SetVolume(float volume)
	{
		OAL_Source_SetGain((*this)->id, volume);
	}
	
	float AudioClip::GetVolume()
	{
		return OAL_Source_GetGain((*this)->id);
	}

	void AudioClip::SetPitch(float pitch)
	{
		OAL_Source_SetPitch((*this)->id, pitch);
	}

	float AudioClip::GetPitch()
	{
		return OAL_Source_GetPitch((*this)->id);
	}

	void AudioClip::SetLoop(bool loop)
	{
		OAL_Source_SetLoop((*this)->id, loop);
	}

	void AudioClip::SetPriority(UInt32 priority)
	{
		OAL_Source_SetPriority((*this)->id, priority);
	}

	UInt32 AudioClip::GetPriority()
	{
		return OAL_Source_GetPriority((*this)->id);
	}

	void AudioClip::SetElapsedTime(double time)
	{
		OAL_Source_SetElapsedTime((*this)->id, time);
	}

	double AudioClip::GetElapsedTime()
	{
		return OAL_Source_GetElapsedTime((*this)->id);
	}

	double AudioClip::GetTotalTime()
	{
		return OAL_Source_GetTotalTime((*this)->id);
	}

	bool AudioClip::IsPlaying()
	{
		return OAL_Source_IsPlaying((*this)->id);
	}

	//void AudioClip::SetPosition(math::Vector3 position)
	//{
	//	OAL_Source_SetPosition((*this)->, position);
	//}

	//void AudioClip::SetVelocity(math::Vector3 velocity)
	//{
	//	OAL_Source_SetPosition((*this)->, velocity);
	//}

	//void Audio::PlayAudioAtPosition(const AudioClip& Clip, Vector3 pos)
	//{
	//	OAL_Source_SetPosition(clip->id, pos);
	//}

	//void Audio::SetAttributes(vector3 apPos, vector3 apVel, vector3 apForward, vector3 apUpward)
	//{
	//	OAL_Listener_SetAttributes(apPos, apVel, apForward, apUpward);
	//}

}