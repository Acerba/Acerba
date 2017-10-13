#pragma once
#include <Ace/IntTypes.h>
namespace ace
{

	enum class KeyMod
	{
		Invalid = 0,
		Shift, Ctrl, ///< Shift, Ctrl KeyMods.
		Alt, AltGr, Caps, ///< Alt, AltGr, Caps KeyMods.
		Win, Numlock, ///< Win, Numlock KeyMods.
	};

	class Keyboard
	{
	public:

		/**
			@brief Returns the pressed key.
			@return state[SDL_SCAN]
		*/
		static Int32 GetKey(const char* key);
		/**
			@brief Returns the pressed modifier.
			@return KeyMod
		*/
		static KeyMod GetMod();

	};
}