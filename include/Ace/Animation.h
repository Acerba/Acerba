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

		/**
			@brief Create animation from spritesheet
			@param[in] Sheet Spritesheet to load
			@see SpriteSheet
		*/
		Animation(const SpriteSheet& Sheet);
		~Animation();
																//TODO: Is this correct way to tell this??
		/**
			@brief Play animation
			@param[in] animationName Name of the anmiation/frame			
		*/
		void PlayAnimation(const std::string& animationName);

		/**
			@brief Update current frame
			@param[in] sprite Sprite where frame is located
		*/
		void UpdateAnimation(Sprite& sprite);

		/**
			@brief Name for the animation
		*/
		std::string AnimationName;

		/**
			@brief Is animation playing true/false
		*/
		bool isPlaying;

		virtual void OnEvent(AnimationEvent);

	private:
		UInt16 m_frameSpeed;
		AnimationData* m_currentAnimation;
		std::vector<AnimationData> m_animations;

		Image m_spriteSheet;
	};
}