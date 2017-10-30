#include <Ace/Animation.h>
#include <Ace/File.h>
#include <Ace/Path.h>
#include <Ace/Json.h>
#include <Ace/Assert.h>
#include <Ace/Time.h>

#include <algorithm>

namespace ace
{
    void UpdateAnimations(EntityHandle* entity, AnimationComponent& animation)
    {
        animation.Update();
    }

    void Animation::Update()
    {
        EntityManager::ForEach<AnimationComponent>(UpdateAnimations);
    }

	UInt32 animationExists(const std::vector<Animation::AnimationData>& anim, const std::string& name)
	{
        for (UInt32 i = 0; i < anim.size(); ++i)
        {
            if (anim[i].name == name)
            {
                return i;
            }
        }

        return anim.size();
	}

	static UInt32 GetNameIndex(const std::string& name)
	{
        static const UInt8 NUMBERS = 10;
		static const char s_numbers[] = 
        {
			'0', '1', '2', '3', '4',
			'5', '6', '7', '8', '9'
		};

	    UInt16 size = name.size();

        for (UInt16 i = 0; i < size; ++i)
        {
             for (UInt16 c = 0; c < NUMBERS; ++c)
             {
             	if (name[i] == s_numbers[c])
             	{
             		return i;
             	}
             }
        }
       
       return size;
	}


	Animation::AnimationData::AnimationData(const std::string& name) :
		frames(),
		name(name),
		speed(1.0f),
        frameTime(1.0f),
		loop(false)
	{
		
	}

	Animation::AnimationData::~AnimationData()
	{
		
	}

    Animation::Animation() : m_handler(nullptr)
    {

    }

	Animation::Animation(const SpriteSheet& sheet) : m_handler(new AnimationStorage())
	{
		const UInt32 spriteCount = sheet.GetSpriteCount();
        m_handler->animations.reserve(spriteCount);

		for (UInt32 i = 0u; i < spriteCount; ++i)
		{
			const SpriteSheet::SpriteData& sprite = *sheet.GetSprite(i);
			const std::string name = sprite.SpriteName.substr(0, GetNameIndex(sprite.SpriteName));

			const UInt32 index = animationExists(m_handler->animations, name);
			if (index == m_handler->animations.size())
			{
                m_handler->animations.emplace_back(AnimationData(name));
			}

            m_handler->animations[index].frames.emplace_back(sprite);

			// 1. Parse Sprite Name (no index)
			// 2. Group Sprites by Name. (+ index) 
			// 3. Store Groups (animations) inside vector
		}

        m_handler->spriteSheet = sheet.image;
	}

	Animation::~Animation()
	{

	}

	//void Animation::PlayAnimation(const std::string& animationName)
	//{
	//	//Looking trough all animations
	//	for (auto& itr : m_animations)
	//	{
	//		//If current animation has same name as looked animation
	//		if (itr.animation == animationName)
	//		{
    //
	//			itr.currentTime = 0.0f;
	//			itr.currentFrame = 0;
	//			
	//			// Sets current animation
	//			m_currentAnimation = &itr;
    //
	//			break;
    //
	//			//TODO: command for drawing frame somewhere	?? is this still valid?
	//		}
	//	}
	//}
    //
	//void Animation::UpdateAnimation(Sprite& sprite)
	//{
	//	if (m_currentAnimation == nullptr)
	//	{
	//		return;
	//	}
    //
	//	//Update UV
	//	sprite.Texcoord(m_currentAnimation->frames[m_currentAnimation->currentFrame].texcoord);
    //
	//	//Update sprite Size
	//	//sprite.Scale(Vector2(m_currentAnimation->frames[m_currentAnimation->currentFrame].rect.width, m_currentAnimation->frames[m_currentAnimation->currentFrame].rect.height));
	//	
	//	// TODO: Update Sprite UV, Sprite Size
	//}
    //
	//void Animation::OnEvent(AnimationEvent event)
	//{
    //    if (!m_currentAnimation)
    //    {
    //        return;
    //    }
    //
    //    if (m_currentAnimation->currentTime >= event.delta)
    //    {
    //        m_currentAnimation->currentTime = 0.f;
    //        //Check if animation is at end
    //        if (++m_currentAnimation->currentFrame >= m_currentAnimation->frames.size())
    //        {
    //            //Check if animation is Looping animation 
    //            if (m_currentAnimation->loop)
    //            {
    //                m_currentAnimation->currentFrame = 0u;
    //            }
    //            else
    //            {
    //                m_currentAnimation = nullptr;
    //            }
    //        }
    //    }
    //
    //    //event.delta // Delta Time
    //    // TODO: Update Animation
    //}

    Int32 Animation::AnimationStorage::GetAnimationIndex(const std::string& name) const
    {
        for (UInt32 i = 0; i < animations.size(); ++i)
        {
            if (animations[i].name == name)
            {
                return i;
            }
        }

        Logger::LogError("%s Animation doesn't exists!", name.c_str());

        return -1;
    }

    bool Animation::AnimationStorage::UpdateAnimation(Sprite& sprite, float scale, UInt32 index, UInt32 frame, float speed) const
    {
        const AnimationData& data = animations[index];
        float time = data.GetTime(frame, speed);

        frame = data.GetFrame(time);
        bool isPlaying = data.loop ? true : time < data.TotalTime();

        if (!isPlaying)
        {
            frame = data.frames.size() - 1;
        }

        sprite.SetSprite(&data.frames[frame], scale);

        return isPlaying;
    }

    bool Animation::UpdateAnimation(Sprite& sprite, UInt32 index, UInt32 time, float scale) const
    {       
        return (m_handler ? m_handler->UpdateAnimation(sprite, scale, index, time) : false);
    }
    
    bool Animation::UpdateAnimation(Sprite& sprite, const std::string& name, UInt32 time, float scale) const
    {
        Int32 index = GetAnimationIndex(name);

        if (index == -1)
        {
            return false;
        }

        return UpdateAnimation(sprite, index, time, scale);
    }

    Int32 Animation::GetAnimationIndex(const std::string& name) const
    {
        return m_handler ? m_handler->GetAnimationIndex(name) : -1;
    }

    Animation::AnimationData* Animation::GetAnimation(UInt32 index)
    {
        return m_handler ? &m_handler->animations[index] : nullptr;
    }

    Animation::AnimationData* Animation::GetAnimation(const std::string& name)
    {
        Int32 index = GetAnimationIndex(name);

        if (index == -1)
        {
            return nullptr;
        }

        return GetAnimation(index);
    }

    const Texture& Animation::GetTexture() const
    {
        return m_handler->spriteSheet;
    }

    const Animation::AnimationHandler& Animation::GetAnimationStorage() const
    {
        return m_handler;
    }

    Animation::operator bool() const
    {
        return m_handler.operator bool();
    }
}
