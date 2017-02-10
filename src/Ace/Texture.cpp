#include <Ace/Texture.h>
#include <Ace/GraphicsDevice.h>

namespace ace
{
	Texture::Texture()
	{

	}

	Texture::Texture(const Image& image)
	{
		Create(image);
	}

	Texture::~Texture()
	{

	}

	bool Texture::Create(const Image& image)
	{
		return Create(image.Pixels(), image.w, image.h, image.format);
	}

	bool Texture::Create(const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format)
	{
		if (impl == nullptr && (*this = GraphicsDevice::CreateTexture()).impl == nullptr)
		{
			return false;
		}

		GraphicsDevice::UpdateTexture(*this, pixels, w, h, format);
		return true;
	}
}