#include <iostream>
#include <string>
#include <Ace/Audio.h>
#include <Ace/IntTypes.h>
#include <Ace/File.h>

namespace ace
{
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

	void Audio::OpenAudio()
	{
		std::string fileName;
		cOAL_Sample *pSample = NULL;
		pSample = OAL_Sample_Load(fileName);
	}

	void Audio::PlayAudio(cOAL_Sample* pSample)
	{
		if (pSample)
		{
			printf("Playing Sample...\n");
			printf("Channels : %d\nFrequency : %d", pSample->GetChannels(), pSample->GetFrequency());
			OAL_Source_Stop(OAL_ALL);
			int s1 = OAL_Sample_Play(OAL_FREE, pSample, 1.0f, true, 10);
					 OAL_Source_SetPaused(s1, false);
			while (OAL_Source_IsPlaying(s1))
			{

			}
		}
		else
		{
			printf("Error!\n");
		}
	}
}