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
			return KeyCode::ONE;
		}
		if (state[SDL_SCANCODE_2])
		{
			return KeyCode::TWO;
		}
		if (state[SDL_SCANCODE_3])
		{
			return KeyCode::THREE;
		}
		if (state[SDL_SCANCODE_4])
		{
			return KeyCode::FOUR;
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
	}
	KeyMod Keyboard::GetMod()
	{
		SDL_Keymod *mod = new SDL_Keymod;
		*mod = SDL_GetModState();
		if (*mod & KMOD_SHIFT)
		{
			return KeyMod::SHIFT;
		}
		if (*mod & KMOD_CTRL)
		{
			return KeyMod::CTRL;
		}
		if (*mod & KMOD_ALT)
		{
			return KeyMod::ALT;
		}
		if (*mod & KMOD_GUI)
		{
			return KeyMod::WIN;
		}
		if (*mod & KMOD_MODE)
		{
			return KeyMod::ALTGR;
		}
		if (*mod & KMOD_CAPS)
		{
			return KeyMod::CAPS;
		}
		if (*mod & KMOD_NUM)
		{
			return KeyMod::NUMLOCK;
		}
	}
}