#include <SDL_joystick.h>
#include <Ace/Accelerometer.h>

#include <Ace/Log.h>

#include <SDL.h>

namespace ace
{
	struct Accelerometer::AccelerometerImpl
	{
		SDL_Joystick* sdlJoystick;

		AccelerometerImpl()
		{
			UInt32 accelerometer = SDL_NumJoysticks() - 1; // Last joystick SHOULD be Accelerometer.

			// Logic for searching Accelerometer if needed.
			//for(int i = 0; i < SDL_NumJoysticks(); i++)
			//{
			//	const char* name = SDL_JoystickNameForIndex(i);
			//
			//	if(strstr(name, "Accelerometer") != nullptr)
			//	{
			//		accelerometer = i;
			//		break;
			//	}
			//}

			sdlJoystick = SDL_JoystickOpen(accelerometer);
		}

		~AccelerometerImpl()
		{
			if(sdlJoystick && SDL_JoystickGetAttached(sdlJoystick))
			{
				SDL_JoystickClose(sdlJoystick);
			}
		}
	};

	Accelerometer* Accelerometer::GetAccelerometer()
	{
		static Accelerometer s_accelerometer;
		return &s_accelerometer;
	}

	Accelerometer::Accelerometer() : m_accelerometerImpl(new Accelerometer::AccelerometerImpl())
	{

	}

	Accelerometer::~Accelerometer()
	{
		if(m_accelerometerImpl)
		{
			delete m_accelerometerImpl;
		}
	}

	float Accelerometer::GetAxis(Int32 axis)
	{
		SDL_JoystickUpdate();
		return SDL_JoystickGetAxis(GetAccelerometer()->m_accelerometerImpl->sdlJoystick, axis) / 32767.0f;
	}
}
