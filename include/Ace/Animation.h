#pragma once
#include <Ace/IntTypes.h>
#include <string>

namespace ace 
{
	class Animation
	{
	public:
		struct AnimationData
		{
			std::string fileName;
			Int32 sizeX;
			Int32 sizeY;
			Int32 loopSpeed;

			Int32 frameSizeX;
			Int32 frameSizeY;

			UInt8* frames;
			UInt8 frameCount;
			AnimationData();
			~AnimationData();
		};

		Animation(const char* jsonName);
		~Animation();


	private:
		UInt16 m_frameSpeed;
		AnimationData m_data;

	};
}