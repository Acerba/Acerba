#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/GraphicsDevice.h>

namespace ace
{
	Window::Window(const char* title, UInt16 w, UInt16 h) : m_windowImpl(new Window::WindowImpl(title, w, h))
	{

	}

	Window::~Window()
	{

	}

	void Window::Clear(const Color32& color)
	{
		GraphicsDevice::Clear(color);
	}

	void Window::Present()
	{
		GraphicsDevice::Present(*this);
	}
}