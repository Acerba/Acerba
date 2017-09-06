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

			switch (i)
			{
			case SDL_WINDOWEVENT: 
				EventManager<WindowEvent>::Broadcast({ static_cast<WindowEventType>(event.window.event), event.window.data1, event.window.data2});
				break;

			case SDL_MOUSEMOTION: 
				EventManager<MouseEvent>::Broadcast({ MouseEventType::Motion, event.motion.which, 0, static_cast<UInt8>(event.motion.state), 0, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, 0 });
				break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				EventManager<MouseEvent>::Broadcast({ i == SDL_MOUSEBUTTONDOWN ? MouseEventType::Pressed : MouseEventType::Released, event.button.which, static_cast<UInt8>(event.button.state), static_cast<UInt8>(event.button.clicks), static_cast<UInt8>(event.button.x), static_cast<UInt8>(event.button.y), 0, 0, 0});
				break;

			case SDL_MOUSEWHEEL:
				EventManager<MouseEvent>::Broadcast({ MouseEventType::Wheel, event.wheel.which, 0, 0, 0, event.wheel.x, event.wheel.y, 0, 0, event.wheel.direction });
				break;

			case SDL_FINGERMOTION:
				EventManager<TouchEvent>::Broadcast({ TouchEventType::Motion, static_cast<UInt8>(event.tfinger.fingerId), event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy, event.tfinger.pressure});
				break;

			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
				EventManager<TouchEvent>::Broadcast({ i == SDL_FINGERDOWN ? TouchEventType::Pressed : TouchEventType::Released, static_cast<UInt8>(event.tfinger.fingerId), event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy, event.tfinger.pressure });
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
            EventManager<Event::SDLEventArg>::Broadcast({ event });
        }
    }
}
