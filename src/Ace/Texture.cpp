#include <Ace/Texture.h>
#include <Ace/GraphicsDevice.h>

namespace ace
{
	Texture::Texture(TextureImpl* impl) : Graphics(impl), size(), scale(1)
	{

	}

	Texture::Texture(const Image& image) : Graphics(nullptr), size(), scale(1)
	{
		Create(image);
	}

	Texture::~Texture()
	{

	}

	bool Texture::Create(const Image& image)
	{
		return Create(image.GetPixelData(), image.w, image.h, image.format, image.scale);
	}

	bool Texture::Create(const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format, float pixelScale)
	{
		if (!(*this) && !(*this = GraphicsDevice::CreateTexture()))
		{
			return false;
		}

		GraphicsDevice::UpdateTexture(*this, pixels, w, h, format); 
        scale = pixelScale;
        size.x = w;
        size.y = h;
		return true;
	}
}