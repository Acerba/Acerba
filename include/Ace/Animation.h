#pragma once
#include <Ace/IntTypes.h>
#include <Ace/SpriteSheet.h>
#include <Ace/Sprite.h>
#include <Ace/Event.h>
#include <Ace/Time.h>
#include <Ace/BaseComponent.h>

#include <Ace/Entity.h>

#include <string>
#include <math.h>

namespace ace 
{
	class Animation final
	{
	public:

        static void Update();

		struct AnimationData final
		{
			std::vector<SpriteSheet::SpriteData> frames;
			std::string name;

            float frameTime;
			float speed;
			bool loop;

			AnimationData(const std::string& name);
			~AnimationData();

            inline float TotalTime() const
            {
                return frameTime * frames.size();
            }

            inline float GetTime(UInt32 index, float animationSpeed) const
            {
                return index * animationSpeed * speed;
            }

            inline UInt32 GetFrame(float time) const
            {
                return fmod(time, TotalTime());
            }
		};

        struct AnimationStorage final
        {
            Texture spriteSheet;
            std::vector<AnimationData> animations;

            Int32 GetAnimationIndex(const std::string& name) const;
            bool UpdateAnimation(Sprite& sprite, float scale, UInt32 index, UInt32 time, float speed = 1.0f) const;
        };

        typedef std::shared_ptr<AnimationStorage> AnimationHandler;

		Animation();

		/**
			@brief Create animation from spritesheet
			@param[in] Sheet Spritesheet to load
			@see SpriteSheet
		*/
		Animation(const SpriteSheet& Sheet);
		~Animation();

		/**
			@brief Update Animation
			@param[in] sprite Sprite
            @param[in] index of animation
            @param[in] frame of animation
		*/
        bool UpdateAnimation(Sprite& sprite, UInt32 index, UInt32 frame, float scale = 1.0f) const;

        /**
            @brief Update Animation
            @param[in] sprite Sprite
            @param[in] name of animation
            @param[in] frame of animation
        */
        bool UpdateAnimation(Sprite& sprite, const std::string& name, UInt32 frame, float scale = 1.0f) const;

        /**
            @brief Gets Animation Index
            @param[in] name of animation
            @return if animation exists returns index else -1
        */
        Int32 GetAnimationIndex(const std::string& name) const;

        /**
            @brief Gets Animation
            @param[in] index of animation
            @return AnimationData
            @see AnimationData

        */
        AnimationData* GetAnimation(UInt32 index);

        /**
            @brief Gets Animation
            @param[in] name of animation
            @return AnimationData
            @see AnimationData
        */
        AnimationData* GetAnimation(const std::string& name);


        /**
            @brief Gets SpriteSheet
        */
        const Texture& GetTexture() const;
        const AnimationHandler& GetAnimationStorage() const;

        operator bool() const;

    private:

        AnimationHandler m_handler;

	};

    struct AnimationComponent final : public BaseComponent
    {
        Animation::AnimationHandler handler;
        ComponentHandle<Sprite>* sprite;

        UInt32 animationIndex;
        UInt32 frame;
        float speed;
        float scale;
        bool isPlaying;

        AnimationComponent(Entity& entity, const Animation& animation) : BaseComponent(entity), sprite(nullptr), handler(animation.GetAnimationStorage()), speed(1.0f), scale(1.0f), isPlaying(false), frame(0)
        {
            sprite = entity.GetComponent<Sprite>();
        }

        inline void Play(UInt32 index)
        {
            if (sprite == nullptr)
            {
                sprite = entity->GetComponentHandle<Sprite>();

                if (sprite == nullptr)
                {
                    return;
                }
            }

            animationIndex = index;
            frame = 0;
            isPlaying = true;
        }

        inline void Play(const std::string& name)
        {
            if (handler)
            {
                Int32 index = handler->GetAnimationIndex(name);

                if (index == -1)
                {
                    return;
                }

                Play(index);
            }
        }

        inline void Pause()
        {
            isPlaying = !isPlaying;
        }

        inline void Stop()
        {
            isPlaying = false;
            frame = 0;
        }

        inline void Update()
        {
            if (sprite != nullptr && isPlaying && handler)
            {
                if (!handler->UpdateAnimation(sprite->GetRef(), scale, animationIndex, frame++, speed))
                {
                    Stop();
                }
            }
        }
    };
}
