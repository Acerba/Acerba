#include <Ace/Audio.h>
#include <Ace/Assert.h>
#include <Ace/IntTypes.h>
#include <Ace/Time.h>
#include <Ace/Log.h>

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>

#include <SDL_thread.h>

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

	struct IAudioSample::AudioClipImpl
	{
		cOAL_Stream* stream;
		cOAL_Sample* sample;

		int id;

		AudioClipImpl(cOAL_Stream* clip) : stream(clip), id(-1), sample(nullptr)
		{

		}

		AudioClipImpl(cOAL_Sample* clip) : sample(clip), id(-1), stream(nullptr)
		{

		}
	};


	IAudioSample::IAudioSample(IAudioSample::AudioClipImpl* impl) : impl(impl)
	{

	}

	IAudioSample::AudioClipImpl* IAudioSample::operator->() const
	{
		return impl.get();
	}

	static cOAL_Sample* LoadSample(const File& file)
	{
		// returns nullptr if audio is not initialized.
		if (!g_isAudioRunning || !File::Exists(file.path))
		{
			return nullptr;
		}

		UInt32 pos = file.Size();
		UInt8* buffer = new UInt8[pos];

		file.Read(buffer, pos);

		cOAL_Sample* sample = OAL_Sample_LoadFromBuffer(buffer, pos);

		if (sample == nullptr)
		{
			Logger::LogError("Audioclip not found %s!", file.path.GetPath().c_str());
		}

		delete buffer;
		return sample;
	}

	static cOAL_Stream* LoadStream(const File& file)
	{
		// returns nullptr if audio is not initialized.
		if (!g_isAudioRunning || !File::Exists(file.path))
		{
			return nullptr;
		}
		
		UInt32 pos = file.Size();
		UInt8* buffer = new UInt8[pos];

		file.Read(buffer, pos);

		cOAL_Stream* stream = OAL_Stream_LoadFromBuffer(buffer, pos);

		if (stream == nullptr)
		{
			Logger::LogError("Audioclip not found %s!", file.path.GetPath().c_str());
		}

		delete buffer;
		return stream;
	}


	AudioClip::AudioClip() : IAudioSample(nullptr)
	{

	}

	AudioClip::AudioClip(const File& file, float volume, bool isPaused, bool loop) : IAudioSample(new AudioClipImpl(LoadSample(file)))
	{
		this->volume = volume;
		this->ispaused = isPaused;
		this->loop = loop;
	}

	AudioStream::AudioStream() : IAudioSample(nullptr)
	{

	}

	AudioStream::AudioStream(const File& file, float volume, bool isPaused, bool loop) : IAudioSample(new AudioClipImpl(LoadStream(file)))
	{
		this->volume = volume;
		this->ispaused = isPaused;
		this->loop = loop;
	}


	void Audio::Init(bool externalInit)
	{
		if (g_isAudioRunning)
		{
			return;
		}

		g_isAudioRunning = true;


		if (!externalInit)
		{

			cOAL_Init_Params oal_parms;

			//ACE_ASSERT(OAL_Init(oal_parms), "Audio initializing failed!", "");

			if (!OAL_Init(oal_parms))
			{
				std::string error = OAL_GetALCErrorString();
				Logger::LogError("Audio initialization failed: %s", error.c_str());
				g_isAudioRunning = false;
			}
		}

		if (g_isAudioRunning)
		{
			g_audioThread = SDL_CreateThread(AudioUpdate, "Audio", nullptr);
		}
	}

	void Audio::Quit()
	{
		if (g_isAudioRunning)
		{
			g_isAudioRunning = false;
			SDL_WaitThread(g_audioThread, nullptr);
			g_audioThread = nullptr;

			OAL_Close();
		}
	}

	bool Audio::Update(const AudioClip& clip)
	{
		if(!g_isAudioRunning)
		{
			return false;
		}
		
		if (clip->id != -1)
		{
			return false;
		}

		return OAL_Source_IsPlaying(clip->id);
	}

	void AudioStream::Play()
	{		
		Audio::PlayAudio(*this);
	}

	void AudioClip::Play()
	{		
		Audio::PlayAudio(*this);
	}

	void Audio::PlayAudio(AudioClip& clip)
	{
		if(!g_isAudioRunning || clip.impl == nullptr)
		{
			return;
		}
		
		if (clip.impl->sample)
		{
			clip.impl->id = OAL_Sample_Play(OAL_FREE, clip.impl->sample, clip.volume, clip.ispaused, 0);
			Audio::GetAudio().clips.push_back(clip);

			OAL_Source_SetLoop(clip->id, clip.loop);

			OAL_Source_SetPaused(clip->id, false);
		}
		else
		{
			Logger::LogError("Audioclip play failed!");
		}
	}

	void Audio::PlayAudio(AudioStream& stream)
	{
		if (!g_isAudioRunning || stream.impl == nullptr)
		{
			return;
		}
		
		if (stream.impl->stream)
		{
			if (stream.impl->id != -1)
			{
				stream.SetElapsedTime(0);
			}
			else
			{
				stream.impl->id = OAL_Stream_Play(OAL_FREE, stream.impl->stream, stream.volume, stream.ispaused);

				OAL_Source_SetLoop(stream->id, stream.loop);

				OAL_Source_SetPaused(stream->id, false);
			}

		}
		else
		{
			Logger::LogError("Audiostream play failed!");
		}
	}

	void Audio::PauseAudio(AudioClip& clip)
	{
		if (!g_isAudioRunning || clip.impl == nullptr)
		{
			return;
		}
		
		clip.ispaused = !clip.ispaused;
		PauseAudio(clip, clip.ispaused);
	}

	void Audio::PauseAudio(AudioClip& clip, bool pause)
	{
		if (!g_isAudioRunning || clip.impl == nullptr)
		{
			return;
		}
		
		OAL_Source_SetPaused(clip->id, pause);
	}

	void Audio::StopAudio(const AudioClip& clip)
	{
		if (!g_isAudioRunning || clip.impl == nullptr)
		{
			return;
		}
		
		OAL_Source_Stop(clip->id);
	}

	void Audio::StopAll()
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_Stop(OAL_ALL);
	}

	void Audio::SetMasterVolume(float volume)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Listener_SetMasterVolume(volume);
	}

	void IAudioSample::SetVolume(float volume)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetGain((*this)->id, volume);
	}

	float IAudioSample::GetVolume()
	{
		if(!g_isAudioRunning)
		{
			return 0;
		}
		
		return OAL_Source_GetGain((*this)->id);
	}

	void IAudioSample::SetPitch(float pitch)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetPitch((*this)->id, pitch);
	}

	float IAudioSample::GetPitch()
	{
		if(!g_isAudioRunning)
		{
			return 0;
		}
		
		return OAL_Source_GetPitch((*this)->id);
	}

	void IAudioSample::SetLoop(bool loop)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetLoop((*this)->id, loop);
	}

	//void AudioClip::SetPriority(UInt32 priority)
	//{
	//	OAL_Source_SetPriority((*this)->id, priority);
	//}

	//UInt32 AudioClip::GetPriority()
	//{
	//	return OAL_Source_GetPriority((*this)->id);
	//}

	void IAudioSample::SetElapsedTime(double time)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetElapsedTime((*this)->id, time);
	}

	double IAudioSample::GetElapsedTime()
	{
		if(!g_isAudioRunning)
		{
			return 0;
		}
		
		return OAL_Source_GetElapsedTime((*this)->id);
	}

	double IAudioSample::GetTotalTime()
	{
		if(!g_isAudioRunning)
		{
			return 0;
		}
		
		return OAL_Source_GetTotalTime((*this)->id);
	}

	bool IAudioSample::IsPlaying()
	{
		if(!g_isAudioRunning)
		{
			return false;
		}
		
		return OAL_Source_IsPlaying((*this)->id);
	}

	void IAudioSample::SetPosition(math::Vector3 position)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetPosition((*this)->id, position.array);
	}

	void IAudioSample::SetVelocity(math::Vector3 velocity)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetPosition((*this)->id, velocity.array);
	}

	void AudioClip::Fade(float duration, float endVolume)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
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
		if(!g_isAudioRunning)
		{
			return;
		}
		
		Audio& audio = Audio::GetAudio();

		audio.effects.push_back(effect);
	}

	void Audio::PlayAudioAtPosition(const AudioClip& Clip, math::Vector3 pos)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Source_SetPosition(Clip->id, pos.array);
	}

	void Audio::SetAttributes(math::Vector3 apPos, math::Vector3 apVel, math::Vector3 apForward, math::Vector3 apUpward)
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
		OAL_Listener_SetAttributes(apPos.array, apVel.array, apForward.array, apUpward.array);
	}

	void Audio::Update()
	{
		if(!g_isAudioRunning)
		{
			return;
		}
		
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