#pragma once

namespace ace
{
	class Accelerometer
	{
		Accelerometer();
		~Accelerometer();
		static void CheckAcclrm(SDL_Joystick *Accelmeter);
		static Sint32 GetAxis(Sint32 axis);
	};
}