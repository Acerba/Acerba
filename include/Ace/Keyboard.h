#pragma once

namespace ace
{
	enum class KeyCode
	{
		W = 1, 
		A,
		S,
		D,
	};
	class Keyboard
	{
	public:
		static KeyCode GetKey();
	};
}