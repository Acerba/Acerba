#include <SDL_joystick.h>
#include <SDL.h>
#include <Ace/Accelerometer.h>
#include <iostream>
namespace ace
{
	Accelerometer::Accelerometer()
	{
		SDL_Joystick *Accelerometer;
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		Accelerometer = SDL_JoystickOpen(0);
	}

	void Accelerometer::CheckAcclrm(SDL_Joystick *Accelerometer)
	{
		if (SDL_NumJoysticks() > 0)
		{
			if (Accelerometer)
			{
				printf("Opened Joystick 0\n");
				printf("Name: %s\n", SDL_JoystickNameForIndex(0));
				printf("Number of Axes: %d\n", SDL_JoystickNumAxes(Accelerometer));
				printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(Accelerometer));
			}
			else
			{
				printf("Couldn't open Joystick 0\n");
			}
		}
		if (SDL_JoystickGetAttached(Accelerometer))
		{
			SDL_JoystickClose(Accelerometer);
		}
	}
	Sint32 Accelerometer::GetAxis(Sint32 axis)
	{
		//axis = 0 for x_axis, axis = 1 for y_axis
		SDL_Joystick *Accelerometer;

		if (axis == 0) //x_axis
		{
			return SDL_JoystickGetAxis(Accelerometer, 0);
		}

		if (axis == 1) //y_axis
		{
			return SDL_JoystickGetAxis(Accelerometer, 1);
		}
		
		else
		{
			printf("GetAxis() axis must be 0 or 1!\n");
		}
	}
}