#include <Ace/Event.h>

#include <SDL_events.h>

namespace ace
{
	Event& Event::GetEvent()
	{
		static Event s_event;
		return s_event;
	}

	Event::Event()
	{
		
	}

	void Event::Update()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			// TODO: Event Signal
		}
	}
}