#pragma once

#include <Ace/Event.h>

#include <SDL_events.h>

namespace ace
{
	struct Event::SDLEvent
	{
		SDL_Event event;
	};
}