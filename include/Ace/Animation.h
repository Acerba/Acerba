#pragma once
#include <Ace/IntTypes.h>
#include <Ace/SpriteSheet.h>
#include <string>

namespace ace 
{
	class Animation
	{
	public:
		struct AnimationData
		{
			std::string animation;
			Int32 loopSpeed;
			std::vector<SpriteSheet::SpriteData> frames;
			AnimationData(const std::string& name);
			~AnimationData();

			float currentTime;
			UInt16 currentFrame;

		};

		Animation(const SpriteSheet& Sheet);
		~Animation();

		void PlayAnimation(const std::string& animationName);

		std::string AnimationName;
	private:
		UInt16 m_frameSpeed;
		std::vector<AnimationData> m_animations;



	};
}