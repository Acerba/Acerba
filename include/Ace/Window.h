#pragma once

#include <Ace/IntTypes.h>
#include <memory>

namespace ace
{
	class Window
	{
		struct WindowImpl;

	public:

		Window(const char* title, UInt16 w, UInt16 h);
		~Window();

	private:

		std::shared_ptr<WindowImpl> m_windowImpl;

	};

	void Log(const char* msg);
}