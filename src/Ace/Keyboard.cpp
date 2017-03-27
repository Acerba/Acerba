#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <SDL_events.h>
#include <Ace/Keyboard.h>
#include <Ace/IntTypes.h>
#include <iostream>
namespace ace
{
	KeyCode Keyboard::GetKey()
	{
		const UInt8 *state = SDL_GetKeyboardState(NULL);

		if (state[SDL_SCANCODE_W])
		{
			return KeyCode::W;
		}
		if (state[SDL_SCANCODE_A])
		{
			return KeyCode::A;
		}
		if (state[SDL_SCANCODE_S])
		{
			return KeyCode::S;
		}
		if (state[SDL_SCANCODE_D])
		{
			return KeyCode::D;
		}
		if (state[SDL_SCANCODE_1])
		{
			return KeyCode::One;
		}
		if (state[SDL_SCANCODE_2])
		{
			return KeyCode::Two;
		}
		if (state[SDL_SCANCODE_3])
		{
			return KeyCode::Three;
		}
		if (state[SDL_SCANCODE_4])
		{
			return KeyCode::Four;
		}
		if (state[SDL_SCANCODE_F1])
		{
			return KeyCode::F1;
		}
		if (state[SDL_SCANCODE_F2])
		{
			return KeyCode::F2;
		}
		if (state[SDL_SCANCODE_F3])
		{
			return KeyCode::F3;
		}
		if (state[SDL_SCANCODE_F4])
		{
			return KeyCode::F4;
		}
		return KeyCode::Invalid;
	}
	KeyMod Keyboard::GetMod()
	{
		SDL_Keymod *mod = new SDL_Keymod;
		*mod = SDL_GetModState();
		if (*mod & KMOD_SHIFT)
		{
			return KeyMod::Shift;
		}
		if (*mod & KMOD_CTRL)
		{
			return KeyMod::Ctrl;
		}
		if (*mod & KMOD_ALT)
		{
			return KeyMod::Alt;
		}
		if (*mod & KMOD_GUI)
		{
			return KeyMod::Win;
		}
		if (*mod & KMOD_MODE)
		{
			return KeyMod::AltGr;
		}
		if (*mod & KMOD_CAPS)
		{
			return KeyMod::Caps;
		}
		if (*mod & KMOD_NUM)
		{
			return KeyMod::Numlock;
		}
		return KeyMod::Invalid;
	}
}