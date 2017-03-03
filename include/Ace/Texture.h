#pragma once

#include <Ace/Image.h>
#include <Ace/TextureFlags.h>

#include <memory>

namespace ace
{
	/**
		@brief Graphics Texture
	*/
	struct Texture
	{
		struct TextureImpl;
		std::shared_ptr<TextureImpl> impl;

		TextureFlags flags;

		/**
			@brief Default Constructor
			Creates an empty Texture.
		*/
		Texture();


		/**
			@brief Initialization Constructor
			Creates texture from a image.
		*/
		Texture(const Image& image);
		~Texture();

		/**
			@brief Create Texture from a image
			@return True if image is valid.
		*/
		bool Create(const Image& image);

		/**
			@brief Create Texture from parameters.
			@param[in] pixels Pixel Data
			@param[in] w Width
			@param[in] h Height
			@param[in] format PixelFormat
			@return True if parameters are valid.
		*/
		bool Create(const UInt8* pixels, UInt32 width, UInt32 height, PixelFormat format);
	};
}