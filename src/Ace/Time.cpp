#include"ace\Time.h"
#include"SDL_timer.h"


namespace ace
{
	UInt32 Time::GetTicks()
	{
		return SDL_GetTicks();
	}

	void Time::Delay(UInt32 ms)
	{
		return SDL_Delay(ms);
	}

	UInt64 Time::GetPerformanceCounter()
	{
		return SDL_GetPerformanceCounter();
	}

	UInt64 Time::GetPerformanceFrequency()
	{
		return SDL_GetPerformanceFrequency();
	}

}