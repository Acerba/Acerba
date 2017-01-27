#include <Ace/Event.h>
#include <Ace/EventManager.h>
#include <Ace/SDLEventArg.h>

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

            EventManager<Event::SDLEventArg>::Broadcast({ event });

        }
    }
}
