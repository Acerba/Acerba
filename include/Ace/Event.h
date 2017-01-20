#pragma once

#include <Ace/Macros.h>

#include <math.h>

namespace ace
{
	class Event
	{
	private:

		static Event& GetEvent();

		Event();

		ACE_DISABLE_COPY(Event)

	public:

		static void Update();

	};
}