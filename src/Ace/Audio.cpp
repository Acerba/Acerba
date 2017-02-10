#include <iostream>
#include <string>
#include <Ace/Audio.h>
#include <Ace/IntTypes.h>

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>


namespace ace
{
	struct AudioSample::AudioSampleImpl
	{
		cOAL_Sample* sample;
		int id;
		AudioSampleImpl(cOAL_Sample* sample) : sample(sample)
		{

		}
	};

	AudioSample::AudioSample(const File& file)
	{
		UInt32 pos = file.Size();
		UInt8* buffer = new UInt8[pos];

		file.Read(buffer, pos);
		impl.reset(new AudioSample::AudioSampleImpl(OAL_Sample_LoadFromBuffer(buffer, pos)));

		delete buffer;
	}


	void Audio::Init()
	{
		printf("Initializing OpenAL.\n");
		fflush(stdout);
		OAL_SetupLogging(true, eOAL_LogOutput_File, eOAL_LogVerbose_High);
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

	bool Audio::Update(const AudioSample& sample)
	{
		return OAL_Source_IsPlaying(sample->id);
	}

	void Audio::PlayAudio(const AudioSample& sample)
	{
		if (sample->sample)
		{
			printf("Playing Sample...\n");
			printf("Channels : %d\nFrequency : %d", sample->sample->GetChannels(), sample->sample->GetFrequency());
			OAL_Source_Stop(OAL_ALL);
			sample->id = OAL_Sample_Play(OAL_FREE, sample->sample, 1.0f, true, 10);
			OAL_Source_SetPaused(sample->id, false);

		}
		else
		{
			printf("Error!\n");
		}
	}
}