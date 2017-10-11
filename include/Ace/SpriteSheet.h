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
            @brief sprite location
            */
            Rect location;

			/**
				@brief texcoord Sprite's texture coordinates
			*/
			Rect texcoord;
		};

		SpriteSheet();


		/**
			@brief SpriteSheet constructor
			@param[in] jsonName Json file name
		*/
		SpriteSheet(const Path& path);
        SpriteSheet(const Image& image);
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

        /**
            @brief Adds Sprite to SpriteSheet
            @param[in] name, unique name of sprite
            @param[in] location, (non normalized) location of sprite
        */
        void AddSprite(std::string name, const Rect& location);

	private:
		std::vector<SpriteData> sprites;
	};
}