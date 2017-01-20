#pragma once

#include <Ace\IntTypes.h>

namespace ace
{
	class Time
	{

		static UInt32 GetTicks();
		static void Delay(UInt32 ms);
		static UInt64 GetPerformanceCounter();
		static UInt64 GetPerformanceFrequency();

		// TODO: Deltatime
	
	
	};
}
