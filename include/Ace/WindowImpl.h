#pragma once

#include <Ace/Window.h>

#include <Ace/GL.h>
#include <Ace/EventManager.h>
#include <Ace/Event.h>
#include <Ace/SDLEventArg.h>
#include <Ace/GraphicsDevice.h>

#include <SDL_video.h>
#include <SDL_log.h>

namespace ace
{
	void InitGraphicsDevice();

	void SetGLStatus(bool ok);

	struct Window::WindowImpl : public EventBase<Event::SDLEventArg>
	{
		SDL_Window* sdlWindow;
		SDL_GLContext context;
		bool isClosed;

		WindowImpl(const char* title, UInt16 w, UInt16 h) : sdlWindow(SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)), isClosed(false)
		{
			// "Pre" Init

			context = SDL_GL_CreateContext(sdlWindow);
			SDL_GL_MakeCurrent(sdlWindow, context);

			// "Post" Init

			#if ACE_WIN

			if (gl3wInit())
			{
				SetGLStatus(false);
			}
			else
			{
				SetGLStatus(true);
			}
            
			if (!gl3wIsSupported(2, 0))
			{
            
			}

			#endif

			InitGraphicsDevice();
		}

		~WindowImpl()
		{
			isClosed = true;
			SDL_DestroyWindow(sdlWindow);
			SDL_GL_DeleteContext(context);
		}

		virtual void OnEvent(Event::SDLEventArg eventArg)
		{
			if (eventArg.event.type == SDL_WINDOWEVENT)
			{
				switch (eventArg.event.window.event)
				{

				case SDL_WINDOWEVENT_CLOSE:
					isClosed = true;
					break;

				case SDL_WINDOWEVENT_RESIZED:
					GraphicsDevice::Viewport(eventArg.event.window.data1, eventArg.event.window.data2);
					break;

				default:
					break;
				}
			}
		}
	};
}
