#pragma once

#include <Ace/Texture.h>
#include <Ace/GL.h>

namespace ace
{
	struct Texture::TextureImpl
	{
		UInt32 textureID;

		TextureImpl()
		{
			glGenTextures(1, &textureID);
		}

		~TextureImpl()
		{
			glDeleteTextures(1, &textureID);
		}
	};
}