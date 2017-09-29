#include <Ace/SpriteSheet.h>
#include <Ace/Path.h>
#include <Ace/Json.h>
#include <Ace/Assert.h>

namespace ace
{
	inline std::string ParsePath(const char* path)
	{
		UInt16 fileTypePos = 0;

		// Find '.' in the path
		while (*(path + fileTypePos++) != '.') 
		{

		}

		// Find file name
		while (*(path + fileTypePos) != '/' && *(path + fileTypePos) != '\\')
		{
			--fileTypePos;
		}

		return std::string(path, path + (fileTypePos + 1));
	}
	
	SpriteSheet::SpriteSheet()
	{

	}

	SpriteSheet::SpriteSheet(const Path& path) : image(nullptr, 0, 0, PixelFormat::Unknown)
	{
		Json Jameson;
		std::string jsonName = path.GetPath();

		if (Jameson.Parse(path))
		{
			SpriteData Data;

			auto root = Jameson.document.GetObject();
			ACE_ASSERT(root.HasMember("packer"), "Jsonfile does not have a member: packer", jsonName);
			ACE_ASSERT(root["packer"].HasMember("filename"), "Packer does not have a member: filename", jsonName);
			ACE_ASSERT(root.HasMember("sprites"), "Jsonfile does not have a member: sprites", jsonName);

			const char* fileName = root["packer"]["filename"].GetString();
			ACE_ASSERT(fileName != nullptr, "Jsonfile does not have valid name %s", jsonName);
			
			auto spritecount = root["sprites"].Size();

			std::string path = ParsePath(jsonName.c_str());
			File imageFile(Path(path + fileName, true));
			image = Image(imageFile);
			
			for (int i = 0; i < spritecount; ++i)
			{
				ACE_ASSERT(root["sprites"][i].HasMember("name"), "Sprites does not have a member: name", jsonName);
				ACE_ASSERT(root["sprites"][i].HasMember("x"), "Sprites does not have a member: x", jsonName);
				ACE_ASSERT(root["sprites"][i].HasMember("y"), "Sprites does not have a member: y", jsonName);
				ACE_ASSERT(root["sprites"][i].HasMember("w"), "Sprites does not have a member: w", jsonName);
				ACE_ASSERT(root["sprites"][i].HasMember("h"), "Sprites does not have a member: h", jsonName);



				const char* spriteName = root["sprites"][i]["name"].GetString();
				ACE_ASSERT(spriteName != nullptr, "Sprite does not have valid name %s", jsonName);
			
				float x = root["sprites"][i]["x"].GetUint();
				float y = root["sprites"][i]["y"].GetUint();
				float w = root["sprites"][i]["w"].GetUint();
				float h = root["sprites"][i]["h"].GetUint();
					
				Data.SpriteName = spriteName;

				Data.location = Rect(x, y, w, h);
				Data.texcoord = Rect(x / image.w, y / image.h, w / image.w, h / image.h);

				sprites.push_back(Data);
			}
		}
		else
		{
			//TODO: Error jos json menee mönkään
		}
	}

	SpriteSheet::~SpriteSheet()
	{
		
	}

	UInt32 SpriteSheet::GetSpriteCount() const
	{
		return sprites.size();
	}

	const SpriteSheet::SpriteData* SpriteSheet::GetSprite(const std::string& Spritename) const
	{
		for (const auto& itr : sprites)
		{
			if (Spritename == itr.SpriteName)
			{
				return &itr;
			}
		}

		return nullptr;
	}

	SpriteSheet::SpriteData* SpriteSheet::GetSprite(const std::string& Spritename)
	{
		for (auto& itr : sprites)
		{
			if (Spritename == itr.SpriteName)
			{
				return &itr;
			}
		}

		return nullptr;
	}

	const SpriteSheet::SpriteData* SpriteSheet::GetSprite(UInt32 index) const
	{

		if (index < sprites.size())
		{
			return &sprites[index];
		}

		return nullptr;
	}

	SpriteSheet::SpriteData* SpriteSheet::GetSprite(UInt32 index)
	{

		if (index < sprites.size())
		{
			return &sprites[index];
		}

		return nullptr;
	}
}