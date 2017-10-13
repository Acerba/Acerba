#pragma once

#include <Ace/Types.h>

namespace ace
{

	enum class TouchEventType
	{
		Motion,
		Pressed,
		Released
	};

	enum class WindowEventType
	{
		None,
		Shown,
		Hidden,
		Exposed,
		Moved,
		Resized,
		SizeChanged,
		Minimised,
		Maximised,
		Restored,
		Enter,
		Leave,
		FocusGained,
		FocusLost,
		Close,
		TakeFocus,
		HitTest
	};

	enum class MouseEventType
	{
		Motion,
		Pressed,
		Released,
		Wheel
	};

	
	struct TouchEvent
	{
		TouchEventType type;
		UInt32 fingerID;
		Vector2 position;
		Vector2 direction;
		float pressure;
	};

	struct WindowEvent
	{
		WindowEventType type;
		Int32 data1;
		Int32 data2;
	};

	struct MouseEvent
	{
		MouseEventType type;
		UInt32 which;

		Vector2 position;

		union
		{
			struct
			{
				UInt8 state;  // Merge	(enum?)
				float xrel;
				float yrel;

			} Motion;

			struct
			{
				UInt32 scroll;
			} Wheel;

			struct
			{
				UInt8 button; // Merge	(enum?)
				UInt8 state;  // Merge	(enum?)
				UInt8 clicks;

			} Button;
		};
		
	};

	struct AccelerometerEvent
	{
		UInt8 axis; // Enum?
		Int16 value;
	};

}