#pragma once

#include <Ace/Types.h>

namespace ace
{
	enum class MouseButton
	{
		Left = 1,
		Middle,
		Right,
	};

	class Mouse
	{
	public:

		//int SDL_GetMouseState(int* x, int* y);
		//int SDL_CaptureMouse(bool enabled);
		//Mouse();
		//~Mouse();

		static Vector2 GetPosition();
		static MouseButton GetButton();
	};
}