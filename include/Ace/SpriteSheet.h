#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Rect.h>
#include <Ace/Image.h>

#include <Ace/Path.h>

#include <string>
#include <vector>

namespace ace
{
	/**
		@brief Sprite Sheet
		http://gamedevutils.com
	*/
	class SpriteSheet
	{
	public:
		/**
			@brief SpriteData struct
		*/
		struct SpriteData
		{
			/**
				@brief SpriteName SpriteName
			*/
			std::string SpriteName;

			/**
				@brief texcoord Sprite's texture coordinates
			*/
			Rect texcoord;

			/**
				@brief sprite location
			*/
			Rect location;

		};

		SpriteSheet();


		/**
			@brief SpriteSheet constructor
			@param[in] jsonName Json file name
		*/
		SpriteSheet(const Path& path);
		~SpriteSheet();

		/**
			@brief Spritesheet image
		*/
		Image image;

		/**
			@return Amount of sprites
		*/
		UInt32 GetSpriteCount() const;

		/**
			@return Sprite
			@param[in] SpriteName Name of the sprite
		*/
		const SpriteData* GetSprite(const std::string& SpriteName) const;
		SpriteData* GetSprite(const std::string& SpriteName);	

		/**
			@return Sprite
			@param[in] index Number of the sprite in sprite container
		*/
		const SpriteData* GetSprite(UInt32 index) const;
		SpriteData* GetSprite(UInt32 index);

	private:
		std::vector<SpriteData> sprites;
	};
}