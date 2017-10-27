#include <Ace/Animation.h>
#include <Ace/File.h>
#include <Ace/Path.h>
#include <Ace/Json.h>
#include <Ace/Assert.h>
#include <Ace/Time.h>

#include <algorithm>

namespace ace
{
	UInt32 animationExists(const std::vector<Animation::AnimationData>& anim, const std::string& name)
	{
		for (UInt32 i = 0; i < anim.size(); ++i)
			if (anim[i].animation == name)
				return i;
		return anim.size();
	}

	static UInt32 GetNameIndex(const std::string& name)
	{
		static const char s_numbers[] = {
			'0', '1', '2', '3', '4',
			'5', '6', '7', '8', '9'
		};

		UInt32 size = name.size();

		auto result = std::find_if(std::begin(s_numbers), std::end(s_numbers), [&name](const char c)
		{
			return c == name.back();
		});

		return std::end(s_numbers) == result ? size : std::distance(std::begin(s_numbers), result);

	}


	Animation::AnimationData::AnimationData(const std::string& name) :
		frames(),
		animation(name),
		currentTime(0.f),
		loopSpeed(1),
		currentFrame(0u),
		loop(false)
	{
		
	}

	Animation::AnimationData::~AnimationData()
	{
		
	}

	Animation::Animation(const SpriteSheet& sheet) :
		isPlaying(false),
		m_animations(),
		m_currentAnimation(nullptr),
		m_frameSpeed(30u)
	{
		const UInt32 spriteCount = sheet.GetSpriteCount();
		m_animations.reserve(spriteCount);
		for (UInt32 i = 0u; i < spriteCount; ++i)
		{
			const SpriteSheet::SpriteData& sprite = *sheet.GetSprite(i);
			const std::string name = sprite.SpriteName.substr(0, GetNameIndex(sprite.SpriteName));

			const UInt32 index = animationExists(m_animations, name);
			if (index == m_animations.size())
			{
				m_animations.emplace_back(AnimationData(name));
			}

			m_animations[index].frames.emplace_back(sprite);

			m_spriteSheet = sheet.image;

			// 1. Parse Sprite Name (no index)
			// 2. Group Sprites by Name. (+ index) 
			// 3. Store Groups (animations) inside vector
		}
	}

	Animation::~Animation()
	{

	}

	void Animation::PlayAnimation(const std::string& animationName)
	{
		//Looking trough all animations
		for (auto& itr : m_animations)
		{
			//If current animation has same name as looked animation
			if (itr.animation == animationName)
			{

				itr.currentTime = 0.0f;
				itr.currentFrame = 0;
				
				// Sets current animation
				m_currentAnimation = &itr;

				break;

				//TODO: command for drawing frame somewhere	?? is this still valid?
			}
		}
	}

	void Animation::UpdateAnimation(Sprite& sprite)
	{
		if (m_currentAnimation == nullptr)
		{
			return;
		}

		//Update UV
		sprite.Texcoord(m_currentAnimation->frames[m_currentAnimation->currentFrame].texcoord);

		//Update sprite Size
		//sprite.Scale(Vector2(m_currentAnimation->frames[m_currentAnimation->currentFrame].rect.width, m_currentAnimation->frames[m_currentAnimation->currentFrame].rect.height));
		
		// TODO: Update Sprite UV, Sprite Size
	}

	void Animation::OnEvent(AnimationEvent event)
	{
        if (!m_currentAnimation)
        {
            return;
        }

        if (m_currentAnimation->currentTime >= event.delta)
        {
            m_currentAnimation->currentTime = 0.f;
            //Check if animation is at end
            if (++m_currentAnimation->currentFrame >= m_currentAnimation->frames.size())
            {
                //Check if animation is Looping animation 
                if (m_currentAnimation->loop)
                {
                    m_currentAnimation->currentFrame = 0u;
                }
                else
                {
                    m_currentAnimation = nullptr;
                }
            }
        }

        //event.delta // Delta Time
        // TODO: Update Animation
    }

}
