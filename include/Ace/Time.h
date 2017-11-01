#pragma once

#include <Ace/IntTypes.h>

namespace ace
{
	class Time
	{

	public:

		/**
			@brief Get the number of milliseconds since the Engine initialization.
			@return Ticks
		*/
		static UInt32 GetTicks();

		/**
			@brief Wait a specified number of milliseconds before returning.
			@return Delay
		*/
		static void Delay(UInt32 ms);

		/**
			@brief Get the current value of the high resolution counter.
			@return Performance Counter
		*/
		static UInt64 GetPerformanceCounter();

		/**
			@brief Get the count per second of the high resolution counter.
			@return Performance Frequency
		*/
		static UInt64 GetPerformanceFrequency();

		/**
			@brief Returns elapsed time since the game last updated.
			@return Delta Time
		*/
		static float DeltaTime();

        /**
            @brief Returns total time.
            @return Total Time
        */
        static float TotalTime();

		static void Update();
		
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
