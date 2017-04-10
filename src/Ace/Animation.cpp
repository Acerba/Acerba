#include <Ace/Animation.h>
#include <Ace/File.h>
#include <Ace/Path.h>
#include <Ace/Json.h>
#include <Ace/Assert.h>

namespace ace
{
	Animation::AnimationData::AnimationData() : 
		fileName(""),
		sizeX(0),
		sizeY(0),
		loopSpeed(0),
		frameSizeX(0),
		frameSizeY(0),
		frames(nullptr),
		frameCount(0)
	{
		
	}

	Animation::AnimationData::~AnimationData()
	{
		if (frames)
		{
			delete[] frames;
			frames = nullptr;
		}
	}

	Animation::Animation(const char* jsonName)
	{
		Json json;

		auto s(Path(jsonName).GetPath());
		//auto res(Json::Read(s));
		
		if (json.Parse(true, std::string(jsonName)))
		{
			auto& root = json.document.GetObject();
			bool b = root.HasMember("file");
			ACE_ASSERT(b, "Jsonfile does not have a member: file", "");
			auto& file = root.MemberBegin();
			std::string s(file->name.GetString());
			ACE_ASSERT(s == "file", "Jsonfile does not have a member: file", "");



			//Path p(imageName);
			//File animfile(p);
			//auto data(animfile.ReadAll());
		}
	}

	Animation::~Animation()
	{

	}


}


/*	anim.json
{
"file": {
"fileName": "spritesheet.png",
"sizeX": 124,
"sizeY": 144,
"loopSpeed": 30
},
"frame": {
"sizeX": 36,
"sizeY": 36
},
"framesToAnimate": {
"frames": [0,3]
}
}
*/