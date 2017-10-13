#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <SDL_events.h>
#include <Ace/Keyboard.h>
#include <Ace/IntTypes.h>
#include <Ace/Log.h>

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

	Int32 Keyboard::GetKey(const char* key)
	{
		const UInt8 *state = SDL_GetKeyboardState(NULL);
		Int32 SDL_SCAN = SDL_GetScancodeFromName(key);
		if (SDL_SCAN == 0)
		{
			Logger::LogError("Keyboard::GetKey() Invalid Keycode.\n");
		}
		return state[SDL_SCAN];
	}

}