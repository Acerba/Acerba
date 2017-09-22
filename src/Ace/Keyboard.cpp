#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <SDL_events.h>
#include <Ace/Keyboard.h>
#include <Ace/IntTypes.h>

namespace ace
{
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

	bool Keyboard::KeyPressed(KeyCode code)
	{
		const UInt8 *state = SDL_GetKeyboardState(NULL);
		int SDL_SCAN = 0;
		switch (code)
		{
		case KeyCode::W:
			SDL_SCAN = 26;
			break;
		case KeyCode::A:
			SDL_SCAN = 4;
			break;
		case KeyCode::S:
			SDL_SCAN = 22;
			break;
		case KeyCode::D:
			SDL_SCAN = 7;
			break;
		case KeyCode::One:
			SDL_SCAN = 30;
			break;
		case KeyCode::Two:
			SDL_SCAN = 31;
			break;
		case KeyCode::Three:
			SDL_SCAN = 32;
			break;
		case KeyCode::Four:
			SDL_SCAN = 33;
			break;
		case KeyCode::F1:
			SDL_SCAN = 58;
			break;
		case KeyCode::F2:
			SDL_SCAN = 59;
			break;
		case KeyCode::F3:
			SDL_SCAN = 60;
			break;
		case KeyCode::F4:
			SDL_SCAN = 61;
			break;
		}
		return state[SDL_SCAN];
	}

}