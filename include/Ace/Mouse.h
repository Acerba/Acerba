#pragma once

#include <Ace/Types.h>

namespace ace
{
	enum class MouseButton
	{
		Invalid = 0,
		Left,
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

		/**
			@brief Returns vector2 position for mouse.
			@return Position
		*/
		static Vector2 GetPosition();

		/**
			@brief Returns the pressed mouse button.
			@return MouseButton
		*/
		static MouseButton GetButton();
	};
}