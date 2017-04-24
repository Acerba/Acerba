#include <Ace/Animation.h>
#include <Ace/File.h>
#include <Ace/Path.h>
#include <Ace/Json.h>
#include <Ace/Assert.h>
#include <Ace/Time.h>

#include <algorithm>

namespace ace
{
	Int32 animationExists(const std::vector<Animation::AnimationData>& anim, const std::string& name)
	{
		for (Int32 i = 0; i < anim.size(); ++i)
			if (anim[i].animation == name)
				return i;
		return -1;
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


//		for (UInt32 i = 0u; i < size; ++i)
//		{
//			for (UInt32 c = 0u; c < 10; ++c)
//			{
//				if (name[i] == s_numbers[c])
//				{
//					return i;
//				}
//			}
//		}
//
//		return size;
	}


	Animation::AnimationData::AnimationData(const std::string& name) :
		animation(name),
		loopSpeed(1),
		frames(),
		currentTime(0.f),
		currentFrame(0u)
	{
		
	}

	Animation::AnimationData::~AnimationData()
	{
		
	}

	Animation::Animation(const SpriteSheet& sheet)
	{
		m_animations.reserve(sheet.GetSpriteCount());
		for (UInt16 i = 0u; i < sheet.GetSpriteCount(); ++i)
		{
			const SpriteSheet::SpriteData& sprite = *sheet.GetSprite(i);
			const std::string name = sprite.SpriteName.substr(0, GetNameIndex(sprite.SpriteName));

			Int32 index = animationExists(m_animations, name);

			if (index == -1)
			{
				index = m_animations.size();
				m_animations.emplace_back(AnimationData(name));
			}

			m_animations[index].frames.emplace_back(sprite);

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
				if ((itr.currentTime += Time::DeltaTime()) > (static_cast<float>(itr.loopSpeed) / 1.f))
				{
					itr.currentTime = 0.f;
					if (++itr.currentFrame >= itr.frames.size())
					{
						itr.currentFrame = 0u;
					}

					//TODO: command for drawing frame somewhere

				}
				break;
			}
		}
	}



}