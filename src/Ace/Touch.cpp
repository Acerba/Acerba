#include <Ace/Touch.h>
#include <SDL_touch.h>
#include <iostream>
namespace ace
{
	static const UInt32 MAX_FINGERS = 10;
	static const SDL_Finger* sdlFingers[MAX_FINGERS];

	struct FingerHandler
	{
		FingerHandler()
		{
			for (int i = 0; i < MAX_FINGERS; ++i)
			{
				sdlFingers[i] = nullptr;
			}
		}

		~FingerHandler()
		{
			// TODO: remove?
		}

	} fingerHandler;

	UInt32 Touch::GetCount()
	{
		return SDL_GetNumTouchFingers(SDL_GetTouchDevice(0));
	}

	Touch Touch::GetTouch(UInt32 index)
	{
		Touch touch;
		touch.m_id = index;
		sdlFingers[index] = SDL_GetTouchFinger(SDL_GetTouchDevice(0), index);
		return touch;

		//return *SDL_GetTouchFinger(TouchIndex, id);
	}

	Int64 Touch::GetID() const
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
