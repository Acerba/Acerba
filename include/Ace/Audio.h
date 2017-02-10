#pragma once

namespace ace
{
	class Audio
	{
	public:
		static void Init();
		static void OpenAudio();
		static void PlayAudio();
		static cOAL_Sample *pSample;
	private:
	
	};
	
}