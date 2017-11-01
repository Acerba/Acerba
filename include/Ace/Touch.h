#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Types.h>

namespace ace
{
	class Touch
	{
	private:

		UInt32 m_id;

		Touch(const UInt32 id);

	public:
		
		/**
			@brief Get the number of active fingers on the device.
			@return finger count
		*/
		static UInt32 GetCount();

		/**
			@brief Get the finger object of the given touch, with the given index.
			@param[in] UInt32 index
			@return Touch
		*/
		static Touch GetTouch(UInt8 index);

		/**
			@return Id of finger.
		*/
		UInt32 GetID() const;

		/**
			@return Position of finger.
		*/
		Vector2 GetPosition() const;

		/**
			@return Pressure of finger.
		*/
		float GetPressure() const;
	};
}