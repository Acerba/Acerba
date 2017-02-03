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
		}
		
	}
}