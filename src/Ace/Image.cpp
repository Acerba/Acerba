#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// stb_image_write.h lis‰‰minen
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <Ace/Image.h>

namespace ace
{
	///Loads image from file
	///Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::File& p_file, PixelFormat format) : format(format)
	{
		auto buffer = p_file.ReadAll();
		int comp;
		UInt8* pixels = stbi_load_from_memory(buffer.get(), p_file.Size(), &w, &h, &comp, static_cast<UInt32>(format));
		m_pixels.reset(pixels);

		format = static_cast<PixelFormat>(comp);
	}

	///Loads image where is only pixeldata
	Image::Image(UInt8* pixels, int w, int h, PixelFormat format) : m_pixels(pixels), w(w), h(h), format(format)
	{

	}

	///Makes a "image" from a color (r,g,b,a)
	///Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::Color& p_color) : format(PixelFormat::RGBA), w(1), h(1)
	{ 
		UInt8* pixels = new UInt8[4];

		pixels[0] = p_color.r;
		pixels[1] = p_color.g;
		pixels[2] = p_color.b;
		pixels[3] = p_color.a;

		
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

	const UInt8* Image::Pixels() const
	{
		return m_pixels.get();
	}

	///Saves a file into given path
	void Image::WritePNG(const Path& path) const
	{
		stbi_write_png(path.GetPath().c_str(), w, h, static_cast<int>(format), m_pixels.get(), w*static_cast<int>(format));
	}
}