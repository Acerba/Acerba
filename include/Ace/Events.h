#pragma once

#include <Ace/IntTypes.h>

namespace ace
{

	enum TouchEventType
	{
		Motion,
		Pressed,
		Released
	};

	enum WindowEventType
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

	enum MouseEventType
	{
		Motion,
		Pressed,
		Released,
		Wheel
	};

	
	struct TouchEvent
	{
		TouchEventType type;
		UInt32 FingerID;
		float x;
		float y;
		float dx;
		float dy;
		float pressure;
	};

	struct WindowEvent
	{
		WindowEventType type;
		UInt8 event;
		Int32 data1;
		Int32 data2;
	};

	struct MouseEvent
	{
		MouseEventType type;
		UInt32 Which;
		UInt8 Button;
		UInt8 State;
		UInt8 Clicks;
		Int32 x;
		Int32 y;
		Int32 xRel;
		Int32 yRel;
		UInt32 Direction;
	};

	struct AccelerometerEvent
	{
		UInt8 Axis;
		Int16 Value;
	};

}