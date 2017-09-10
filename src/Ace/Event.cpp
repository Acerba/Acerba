#include <Ace/Event.h>
#include <Ace/EventManager.h>
#include <Ace/SDLEventArg.h>
#include <Ace/Accelerometer.h>

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
			UInt32 i = event.type;

			MouseEvent mouseEvent;

			switch (i)
			{
			case SDL_MOUSEMOTION: 

				mouseEvent.type = MouseEventType::Motion;
				mouseEvent.which = event.motion.which;
				mouseEvent.position.x = event.motion.x;
				mouseEvent.position.y = event.motion.y;

				mouseEvent.Motion.state = event.motion.state;
				mouseEvent.Motion.xrel = event.motion.xrel;
				mouseEvent.Motion.yrel = event.motion.yrel;
				break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:

				mouseEvent.type = i == SDL_MOUSEBUTTONDOWN ? MouseEventType::Pressed : MouseEventType::Released;
				mouseEvent.which = event.button.which;
				mouseEvent.position.x = event.button.x;
				mouseEvent.position.y = event.button.y;

				mouseEvent.Button.button = event.button.button;
				mouseEvent.Button.state = event.button.state;
				mouseEvent.Button.clicks = event.button.clicks;
				break;

			case SDL_MOUSEWHEEL:

				mouseEvent.type = MouseEventType::Wheel;
				mouseEvent.which = event.wheel.which;
				mouseEvent.position.x = event.wheel.x;
				mouseEvent.position.y = event.wheel.y;

				mouseEvent.Wheel.scroll = event.wheel.direction;
				break;

			case SDL_WINDOWEVENT:
				EventManager<WindowEvent>::Broadcast({ static_cast<WindowEventType>(event.window.event), event.window.data1, event.window.data2 });
				break;


			case SDL_FINGERMOTION:
				EventManager<TouchEvent>::Broadcast({ TouchEventType::Motion, static_cast<UInt8>(event.tfinger.fingerId), { event.tfinger.x, event.tfinger.y }, { event.tfinger.dx, event.tfinger.dy }, event.tfinger.pressure });
				break;

			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
				EventManager<TouchEvent>::Broadcast({ i == SDL_FINGERDOWN ? TouchEventType::Pressed : TouchEventType::Released, static_cast<UInt8>(event.tfinger.fingerId), Vector2(event.tfinger.x, event.tfinger.y), Vector2( event.tfinger.dx, event.tfinger.dy), event.tfinger.pressure
			});
				break;

			case SDL_JOYAXISMOTION:
			{
				if (event.jaxis.which != Accelerometer::GetID())
				{
					break;
				}
				EventManager<AccelerometerEvent>::Broadcast({ event.jaxis.axis, event.jaxis.value });
			}
			break;
			}

			switch (i)
			{
			case SDL_MOUSEWHEEL:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				EventManager<MouseEvent>::Broadcast(mouseEvent);
				break;
			}


            EventManager<Event::SDLEventArg>::Broadcast({ event });
        }
    }
}
