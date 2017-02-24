#pragma once

#include <Ace\IntTypes.h>

namespace ace
{
	class Time
	{

	public:

	
		static UInt32 GetTicks();
		static void Delay(UInt32 ms);
		static UInt64 GetPerformanceCounter();
		static UInt64 GetPerformanceFrequency();

		static float DeltaTime();
		
		struct WaitTime
		{
			UInt32 wait;

			WaitTime(UInt32 time)
			{
				wait = Time::GetTicks() + time;
			}
			bool IsDone();
		};
	};
}
