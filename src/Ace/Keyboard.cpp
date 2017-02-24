#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <Ace/Keyboard.h>
#include <Ace/IntTypes.h>
#include <iostream>
namespace ace
{
	KeyCode Keyboard::GetKey()
	{
		const UInt8 *state = SDL_GetKeyboardState(NULL);
		switch (*state)
		{
		case SDL_SCANCODE_W:
			printf("W was pressed!\n");
			return KeyCode::W;
			break;
		case SDL_SCANCODE_A:
			printf("A was pressed!\n");
			return KeyCode::A;
			break;
		case SDL_SCANCODE_S:
			printf("S was pressed!\n");
			return KeyCode::S;
			break;
		case SDL_SCANCODE_D:
			printf("D was pressed!\n");
			return KeyCode::D;
			break;
		case SDL_SCANCODE_1:
			printf("1 was pressed!\n");
			return KeyCode::ONE;
			break;
		case SDL_SCANCODE_2:
			printf("2 was pressed!\n");
			return KeyCode::TWO;
			break;
		case SDL_SCANCODE_3:
			printf("3 was pressed!\n");
			return KeyCode::THREE;
			break;
		case SDL_SCANCODE_4:
			printf("4 was pressed!\n");
			return KeyCode::FOUR;
			break;
		case SDL_SCANCODE_F1:
			printf("F1 was pressed!\n");
			return KeyCode::F1;
			break;
		case SDL_SCANCODE_F2:
			printf("F2 was pressed!\n");
			return KeyCode::F2;
			break;
		case SDL_SCANCODE_F3:
			printf("F3 was pressed!\n");
			return KeyCode::F3;
			break;
		case SDL_SCANCODE_F4:
			printf("F4 was pressed!\n");
			return KeyCode::F4;
			break;
		}
	}

	KeyMod Keyboard::GetMod()
	{
		SDL_Keymod *mod;
		switch (*mod)
		{
		case KMOD_SHIFT:
			printf("SHIFT was pressed!\n");
			return KeyMod::SHIFT;
			break;
		case KMOD_CTRL:
			printf("CTRL was pressed!\n");
			return KeyMod::CTRL;
			break;
		case KMOD_ALT:
			printf("ALT was pressed!\n");
			return KeyMod::ALT;
			break;
		case KMOD_GUI:
			printf("WINKEY was pressed!\n");
			return KeyMod::WIN;
			break;
		case KMOD_CAPS:
			printf("CAPSLOCK was pressed!\n");
			return KeyMod::CAPS;
			break;
		case KMOD_MODE:
			printf("ALTGR was pressed!\n");
			return KeyMod::ALTGR;
			break;
		case KMOD_NUM:
			printf("NUMLOCK was pressed!\n");
			return KeyMod::NUMLOCK;
			break;
		}
	}
}