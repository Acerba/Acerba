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

	Image::~Image()
	{

	}

	///Returns image pixel data
	UInt8* Image::Pixels()
	{
		return m_pixels.get();
	}

}