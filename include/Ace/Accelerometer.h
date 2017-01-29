#pragma once

#include <Ace/Macros.h>
#include <Ace/IntTypes.h>

namespace ace
{
	class Accelerometer
	{
		// Sigleton
		static Accelerometer* GetAccelerometer();

		// Wrapper for SDL_Joystick.
		struct AccelerometerImpl;
		AccelerometerImpl* m_accelerometerImpl;

		ACE_DISABLE_COPY(Accelerometer)

		Accelerometer();
		~Accelerometer();

	public:

		// No SDL_Joystick on headers.
		//static void CheckAcclrm(SDL_Joystick *Accelmeter);

		static float GetAxis(Int32 axis);
	};
}
