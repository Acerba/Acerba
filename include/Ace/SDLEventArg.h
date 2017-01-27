#pragma once

#include <Ace/Event.h>

#include <SDL_events.h>

namespace ace
{
	struct Event::SDLEventArg
	{
		SDL_Event event;
	};
}