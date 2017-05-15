#pragma once
#include <Ace/IntTypes.h>
#include <Ace/SpriteSheet.h>
#include <Ace/Sprite.h>
#include <string>

#include <Ace/Event.h>

namespace ace 
{
	struct AnimationEvent
	{
		float delta;
	};

	class Animation : public EventBase<AnimationEvent>
	{
	public:
		struct AnimationData
		{
			std::string animation;
			Int32 loopSpeed; // Animation speed? Frame speed?
			bool loop;

			std::vector<SpriteSheet::SpriteData> frames;
			AnimationData(const std::string& name);
			~AnimationData();

			float currentTime;
			UInt16 currentFrame;

		};

		Animation(){};

		Animation(const SpriteSheet& Sheet);
		~Animation();

		void PlayAnimation(const std::string& animationName);
		void UpdateAnimation(Sprite& sprite);

		std::string AnimationName;

		bool isPlaying;

		virtual void OnEvent(AnimationEvent);

	private:
		UInt16 m_frameSpeed;
		AnimationData* m_currentAnimation;
		std::vector<AnimationData> m_animations;



	};
}