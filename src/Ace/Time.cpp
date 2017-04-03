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

	static float s_deltaTime;

	float Time::DeltaTime()
	{
		return s_deltaTime;
	}

	void Time::Update()
	{
		//static UInt64 s_now = 0;
		//static UInt64 s_last = 0;
        //
		//s_last = s_now;
		//s_now = GetPerformanceCounter();
        //
		//s_deltaTime = static_cast<float>(((s_now - s_last) / GetPerformanceFrequency())) / 1000.0f;

        static UInt32 s_now = 0;
        static UInt32 s_last = 0;
        
        s_last = s_now;
        s_now = GetTicks();
        
        s_deltaTime = static_cast<float>(s_now - s_last) / 1000.0f;
	}

	bool Time::WaitTime::IsDone()
	{
		return SDL_TICKS_PASSED(Time::GetTicks(),wait);
	}
}