#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/GraphicsDevice.h>

#include <Ace/EventManager.h>
#include <Ace/Event.h>

#include <Ace/SDLEventArg.h>

namespace ace
{
	static Window* g_currentWindow = nullptr;

	Window* Window::GetCurrent()
	{
		return g_currentWindow;
	}

    Window::Window(WindowImpl* impl) : m_windowImpl(impl)
    {
		if (g_currentWindow == nullptr)
		{
			g_currentWindow = this;
		}
    }

    Window::Window(const char* title, UInt16 w, UInt16 h) :
        m_windowImpl(new Window::WindowImpl(title, w, h)),
        m_color(127u, 127u, 127u, 0u)
    {
		if (g_currentWindow == nullptr)
		{
			g_currentWindow = this;
		}
    }

    Window::~Window()
    {
		if (this == g_currentWindow)
		{
			g_currentWindow = nullptr;
		}
    }

    void Window::Close()
    {
        if (m_windowImpl)
        {
            m_windowImpl->isClosed = true;
        }
    }

    void Window::Clear(const Color32* color)
    {
        GraphicsDevice::Clear(color ? *color : m_color.operator Color32());
    }

    void Window::SetClearColor(const Color& color)
    {
        m_color = color;
    }

    void Window::Present()
    {
        GraphicsDevice::Present(*this);
    }

    Vector2 Window::GetSize() const
    {
        Int32 w = 0, h = 0;
        SDL_GetWindowSize(m_windowImpl->sdlWindow, &w, &h);
        return Vector2((float)w, (float)h);
    }

    void Window::SetSize(const Vector2& size)
    {
        SDL_SetWindowSize(m_windowImpl->sdlWindow, size.x, size.y);
    }

    Window::operator bool() const
    {
        return m_windowImpl.operator bool() && m_windowImpl->sdlWindow && !m_windowImpl->isClosed;
    }
}