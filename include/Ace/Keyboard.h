#pragma once

namespace ace
{
	enum class KeyCode
	{
		Invalid = 0,
		W,  A ,   S,    D, ///< W, A, S, D Keycodes.
		One, Two, Three, Four, ///< 1, 2, 3, 4 Keycodes.
		F1,	  F2,   F3,   F4, ///< F1, F2, F3, F4 Keycodes.
	};

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
			@return KeyCode
		*/
		static bool KeyPressed(KeyCode code);
		/**
			@brief Returns the pressed modifier.
			@return KeyMod
		*/
		static KeyMod GetMod();


	};
}