#include <Ace/Audio.h>
#include <Ace/IntTypes.h>
#include <Ace/Time.h>
#include <Ace/Log.h>

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>

#include <SDL_thread.h>


//#include <OALWrapper/OAL_Device.h>
//#include <OALWrapper/OAL_Source.h>
//#include <OALWrapper/OAL_OggSample.h>
//#include <OALWrapper/OAL_WAVSample.h>
//#include <OALWrapper/OAL_OggStream.h>
//#include <OALWrapper/OAL_CustomStream.h>
//#include <OALWrapper/OAL_SourceManager.h>
//
//#include <OALWrapper/OAL_Filter.h>
//#include <OALWrapper/OAL_Helper.h>

namespace ace
{
	static SDL_Thread* g_audioThread;
	static bool g_isAudioRunning;
	
	// Audio Update thread.
	static int AudioUpdate(void* data)
	{
		 Int32 wait = 1000 / 30; // 30times / second
		while (g_isAudioRunning)
		{
			Audio::Update();
			Time::Delay(wait);
		}
		return 0;
	}

	struct AudioClip::AudioClipImpl
	{
		cOAL_Sample* clip;
		int id;

		AudioClipImpl(cOAL_Sample* clip) : clip(clip), id(-1)
		{

		}
	};

	AudioClip::AudioClip(AudioClipImpl* clip) : impl(clip)
	{

	}

	AudioClip::AudioClip(const File& file, float volume, bool loop, UInt32 priority) : volume(volume), loop(loop), priority(priority)
	{
		UInt32 pos = file.Size();
		UInt8* buffer = new UInt8[pos];

		file.Read(buffer, pos);
		impl.reset(new AudioClip::AudioClipImpl(OAL_Sample_LoadFromBuffer(buffer, pos)));

		delete buffer;
	}


	void Audio::Init(bool externalInit)
	{
		if(g_audioThread != nullptr)
		{
			return;
		}

		if(!externalInit)
		{

			cOAL_Init_Params oal_parms;

			if (OAL_Init(oal_parms) == false)
			{
				Logger::LogInfo("Audio initializing failed!");
			}
			else
			{
				Logger::LogInfo("Audio initializing succeeded!");
			}
				
		}

		g_isAudioRunning = true;
		g_audioThread = SDL_CreateThread(AudioUpdate, "Audio", nullptr);
	}

	void Audio::Quit()
	{
		if (g_audioThread)
		{
			g_isAudioRunning = false;
			SDL_WaitThread(g_audioThread, nullptr);
			g_audioThread = nullptr;
		}

		OAL_Close();
	}

	bool Audio::Update(const AudioClip& clip)
	{
		if (clip->id != -1)
		{
			return false;
		}

		return OAL_Source_IsPlaying(clip->id);
	}

	void Audio::PlayAudio(const AudioClip& clip)
	{
		if (clip->clip)
		{
			// TODO: Use Logger instead! (Debug Only)

			printf("Channels : %d\nFrequency : %d\n", clip->clip->GetChannels(), clip->clip->GetFrequency());

			clip->id = OAL_Sample_Play(OAL_FREE, clip->clip, clip.volume, clip.loop, clip.priority);
			OAL_Source_SetPaused(clip->id, false);
			Audio::GetAudio().clips.push_back(clip);
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

	void AudioClip::SetPosition(math::Vector3 position)
	{
		OAL_Source_SetPosition((*this)->id, position.array);
	}

	void AudioClip::SetVelocity(math::Vector3 velocity)
	{
		OAL_Source_SetPosition((*this)->id, velocity.array);
	}

	void AudioClip::Fade(float duration, float endVolume)
	{
		FadeEffect* fade = new FadeEffect(*this, duration, volume, endVolume);
		Audio::AddEffect(fade);
	}

	Audio& Audio::GetAudio()
	{
		static Audio s_audio;
		return s_audio;
	}

	Audio::Audio()
	{

	}

	Audio::~Audio()
	{
		for (UInt32 i = 0u; i < effects.size(); ++i)
		{
			if (effects[i] == nullptr)
			{
				continue;
			}

			delete effects[i];
			effects[i] = nullptr;
		}
	}
	
	void Audio::AddEffect(IAudioEffect* effect)
	{
		Audio& audio = Audio::GetAudio();

		audio.effects.push_back(effect);
	}

	void Audio::PlayAudioAtPosition(const AudioClip& Clip, math::Vector3 pos)
	{
		OAL_Source_SetPosition(Clip->id, pos.array);
	}

	void Audio::SetAttributes(math::Vector3 apPos, math::Vector3 apVel, math::Vector3 apForward, math::Vector3 apUpward)
	{
		OAL_Listener_SetAttributes(apPos.array, apVel.array, apForward.array, apUpward.array);
	}

	void Audio::Update()
	{
		Audio& audio = Audio::GetAudio();
		
		for (UInt32 i = 0u; i < audio.effects.size(); ++i)
		{
			if (audio.effects[i] == nullptr)
			{
				continue;
			}

			float normalized = audio.effects[i]->time / audio.effects[i]->duration;

			if (normalized > 1)
			{
				delete audio.effects[i];
				audio.effects[i] = nullptr;
				continue;
			}

			// TODO: Check that effect clip is playing.

			audio.effects[i]->Effect(normalized);
			audio.effects[i]->time += Time::DeltaTime(); 
		}

		for (UInt32 i = 0u; i < audio.clips.size(); ++i)
		{
			if (!OAL_Source_IsPlaying(audio.clips[i]->id))
			{
				// TODO: Remove clip from the vector. (Or add check for "duplicated" clips in PlayAudio method)
			}
		}
	}
}