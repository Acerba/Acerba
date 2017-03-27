#pragma once

#include <Ace/IntTypes.h>

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
		Resised,
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
		float x;
		float y;
		float dx;
		float dy;
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
		UInt8 button;
		UInt8 state;
		UInt8 clicks;
		Int32 x;
		Int32 y;
		Int32 xRel;
		Int32 yRel;
		UInt32 direction;
	};

	struct AccelerometerEvent
	{
		UInt8 axis;
		Int16 value;
	};

}