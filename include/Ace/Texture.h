#pragma once

#include <Ace/Image.h>
#include <Ace/TextureFlags.h>

#include <memory>

namespace ace
{
	struct Texture
	{
		struct TextureImpl;
		std::shared_ptr<TextureImpl> impl;

		TextureFlags flags;

		Texture();
		Texture(const Image& image);
		~Texture();

		bool Create(const Image& image);
		bool Create(const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format);
	};
}