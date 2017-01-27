#pragma once

#include <Ace/Macros.h>

#include <math.h>

#include <Ace/EventManager.h>

namespace ace
{
	class Event
	{
	public:

		struct SDLEventArg;

	private:

		static Event& GetEvent();

		Event();

		ACE_DISABLE_COPY(Event)

	public:

		static void Update();

	};


    
}