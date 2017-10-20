#include <Ace/Mouse.h>
#include <Ace/IntTypes.h>
#include <Ace/Window.h>
#include <Ace/Camera.h>
#include <SDL_mouse.h>

namespace ace
{

	Vector2 Mouse::GetPosition()
	{
		Vector2 size = Window::GetCurrent()->GetSize();

		Int32 x, y;
		SDL_GetMouseState(&x, &y);
		return Vector2(static_cast<float>(x / size.x), static_cast<float>(y / size.y));
	}

	Vector2 Mouse::GetWorldPosition(ace::Camera camera)
	{
		Int32 x, y;
		SDL_GetMouseState(&x, &y);
		Vector2 MousePosition(static_cast<float>(x), static_cast<float>(y));
		//MousePosition.x = ((MousePosition.x / Window::GetCurrent()->GetSize().x)*(602-228)+228); // * Jotain)
		//MousePosition.y = ((MousePosition.y / Window::GetCurrent()->GetSize().y)*(316-88)+88); // * Jotain)
		MousePosition = ((MousePosition / Window::GetCurrent()->GetSize().x));
		return MousePosition;
	}

	MouseButton Mouse::GetButton()
	{

		UInt32 state = SDL_GetMouseState(NULL, NULL);

		if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			return MouseButton::Left;
		}

		if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			return MouseButton::Middle;
		}

		if (state & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			return MouseButton::Right;
		}

		return MouseButton::Invalid;
	}
}