#include <Ace/Texture.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/GLTextureImpl.h>

namespace ace
{
	Texture::Texture(TextureImpl* impl) : Graphics(impl)
	{

	}

	Texture::Texture(const Image& image) : Graphics(nullptr)
	{
		Create(image);
	}

	Texture::~Texture()
	{

	}

	bool Texture::Create(const Image& image)
	{
		return Create(image.GetPixelData(), image.w, image.h, image.format);
	}

	bool Texture::Create(const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format)
	{
		if (!(*this) && !(*this = GraphicsDevice::CreateTexture()))
		{
			return false;
		}

		GraphicsDevice::UpdateTexture(*this, pixels, w, h, format);
		return true;
	}
}