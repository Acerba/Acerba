#pragma once

#include <Ace/Window.h>

#include <SDL_video.h>
#include <SDL_log.h>

#include <Ace/GL.h>

namespace ace
{
	struct Window::WindowImpl
	{
		SDL_Window* sdlWindow;
		SDL_GLContext context;


		WindowImpl(const char* title, UInt16 w, UInt16 h) : sdlWindow(SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
		{
			// "Pre" Init

			context = SDL_GL_CreateContext(sdlWindow);
			SDL_GL_MakeCurrent(sdlWindow, context);

			// "Post" Init

			if (gl3wInit())
			{
			}

			if (!gl3wIsSupported(3, 0))
			{
			}

		}

		~WindowImpl()
		{
			SDL_DestroyWindow(sdlWindow);
			SDL_GL_DeleteContext(context);
		}
	};
}