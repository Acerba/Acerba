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

		/**
			@brief Get the current state of an axis control on a joystick(Accelerometer).
			@brief The state is a value ranging from -32768 to 32767.
			@return Axis
		*/
		static float GetAxis(Int32 axis);
	};
}
