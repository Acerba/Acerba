#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Color.h>
#include <memory>

namespace ace
{
	class Window
	{

	public:
		struct WindowImpl;

	private:

		std::shared_ptr<WindowImpl> m_windowImpl;

	public:

		Window(const char* title, UInt16 w, UInt16 h);
		~Window();

		inline WindowImpl* operator*()
		{
			return m_windowImpl.get();
		}

		void Present();
		void Clear(const Color32& color);
	};
}