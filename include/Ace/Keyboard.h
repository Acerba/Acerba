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
		static KeyCode GetKey();
		static KeyMod GetMod();
	};
}