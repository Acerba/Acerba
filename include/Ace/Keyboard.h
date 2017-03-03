#pragma once

namespace ace
{
	enum class KeyCode
	{
		W=1,  A ,   S,    D,
		ONE, TWO, THREE, FOUR,
		F1,	  F2,   F3,   F4,
	};

	enum class KeyMod
	{
		SHIFT=1, CTRL,
		ALT, ALTGR, CAPS,
		WIN, NUMLOCK,
	};

	class Keyboard
	{
	public:

		/**
			@brief Returns the pressed key.
			@return Key Code
		*/
		static KeyCode GetKey();

		/**
			@brief Returns the pressed modifier.
			@return Key Mod
		*/
		static KeyMod GetMod();

	};
}