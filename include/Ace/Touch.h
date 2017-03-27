#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Types.h>

namespace ace
{
	class Touch
	{
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
		static Touch GetTouch(UInt32 index);

	private:

		UInt32 m_id;

	public:

		/**
			@brief Returns id of finger.
			@return id
		*/
		Int64 GetID() const;

		/**
			@brief Returns position of finger in Vector2.
			@return Vector2
		*/
		Vector2 GetPosition() const;

		/**
			@brief Returns pressure of finger press.
			@return pressure
		*/
		float GetPressure() const;
	};
}