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

		static UInt32 GetID();

		/**
			@brief Get the current state of an axis control on a Accelerometer.
			The state is a value ranging from -1 to 1.
			@return Axis
		*/
		static float GetAxis(Int32 axis);


		/**
			@brief The state is a value ranging from -1 to 1.
			@return X Axis
		*/
		inline static float GetX()
		{
			return GetAxis(0);
		}

		/**
			@brief The state is a value ranging from -1 to 1.
			@return Y Axis
		*/
		inline static float GetY()
		{
			return GetAxis(1);
		}

		/**
			@brief The state is a value ranging from -1 to 1.
			@return Z Axis
		*/
		inline static float GetZ()
		{
			return GetAxis(2);
		}

	};
}
