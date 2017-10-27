#pragma once
#include <Ace/IntTypes.h>
#include <Ace/SpriteSheet.h>
#include <Ace/Sprite.h>
#include <string>

#include <Ace/Event.h>

namespace ace 
{
	struct AnimationEvent final
	{
		float delta;
	};

	class Animation final : public EventBase<AnimationEvent>
	{
	public:
		struct AnimationData
		{
			std::vector<SpriteSheet::SpriteData> frames;
			std::string animation;
			float currentTime;
			Int32 loopSpeed; // Animation speed? Frame speed?
			UInt16 currentFrame;
			bool loop;

			AnimationData(const std::string& name);
			~AnimationData();
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

		virtual void OnEvent(AnimationEvent) final override;

	private:
		std::vector<AnimationData> m_animations;
		Image m_spriteSheet;
		AnimationData* m_currentAnimation;
		UInt16 m_frameSpeed;

	};
}