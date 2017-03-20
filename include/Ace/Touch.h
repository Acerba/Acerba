#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Types.h>

namespace ace
{
	class Touch
	{
	public:
		static UInt32 GetCount();
		static Touch GetTouch(UInt32 index);

	private:

		UInt32 m_id;

	public:

		Int64 GetID() const;
		Vector2 GetPosition() const;
		float GetPressure() const;
	};
}