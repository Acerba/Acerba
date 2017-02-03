#pragma once
#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>
namespace ace
{
	class Audio
	{
	public:
		static void Init();
		static void OpenAudio();
		static void PlayAudio(cOAL_Sample* pSample);
	};
}