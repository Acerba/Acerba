#include <Ace/Touch.h>
#include <SDL_touch.h>

#include <Ace/Log.h>

namespace ace
{
	const UInt8 MAX_FINGERS = 10u;
	const SDL_Finger* sdlFingers[MAX_FINGERS];

	struct FingerHandler
	{
		FingerHandler()
		{
			for (UInt8 i = 0; i < MAX_FINGERS; ++i)
			{
				sdlFingers[i] = nullptr;
			}
		}

		~FingerHandler()
		{
			// TODO: remove?
		}

	} fingerHandler;

	Touch::Touch(const UInt32 id) : m_id(id)
	{

	}

	UInt32 Touch::GetCount()
	{
		return SDL_GetNumTouchFingers(SDL_GetTouchDevice(0));
	}

	Touch Touch::GetTouch(UInt8 index)
	{
		if (index >= MAX_FINGERS)
		{
			Logger::LogError("Touch::GetTouch() index overflow, using 0");
			index = 0u;
		}
		sdlFingers[index] = SDL_GetTouchFinger(SDL_GetTouchDevice(0), index);
		return Touch(index);
		//return *SDL_GetTouchFinger(TouchIndex, id);
	}

	UInt32 Touch::GetID() const
	{
		return sdlFingers[m_id]->id;
	}

	Vector2 Touch::GetPosition() const
	{
		return Vector2(static_cast<float>(sdlFingers[m_id]->x), static_cast<float>(sdlFingers[m_id]->y));
	}

	float Touch::GetPressure() const
	{
		return sdlFingers[m_id]->pressure;
	}
}
