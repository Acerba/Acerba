#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Ace/Image.h>

namespace ace
{
	///Loads image from file
	///Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::File& p_file, PixelFormat format)
	{
		auto buffer = p_file.ReadAll();
		UInt8* pixels = stbi_load_from_memory(buffer.get(), p_file.Size(), &w, &h, &comp, static_cast<UInt32>(format));
		m_pixels.reset(pixels);
	}

	///Makes a "image" from a color (r,g,b,a)
	///Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::Color& p_color)
	{ 
		UInt8* pixels = new UInt8[4];

		pixels[0] = p_color.r;
		pixels[1] = p_color.g;
		pixels[2] = p_color.b;
		pixels[3] = p_color.a;

		comp = 4;
		m_pixels.reset(pixels);
	}

	Image::~Image()
	{

	}

	///Returns image pixel data
	UInt8* Image::Pixels()
	{
		return m_pixels.get();
	}

}