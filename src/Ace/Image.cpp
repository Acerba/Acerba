#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <Ace/Image.h>

namespace ace
{
	//Loads image from file
	//Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::File& p_file)
	{
		auto buffer = p_file.ReadAll();
		int comp;

		UInt8* pixels = stbi_load_from_memory(buffer.get(), p_file.Size(), &w, &h, &comp, 0);
		m_pixels.reset(pixels);

		this->format = static_cast<PixelFormat>(comp);
	}

	//Loads image where is only pixeldata
	Image::Image(UInt8* pixels, Int32 w, Int32 h, PixelFormat format) : m_pixels(pixels), w(w), h(h), format(format)
	{
		
	}

	//Makes a "image" from a color (r,g,b,a)
	//Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::Color& p_color) :
		format(PixelFormat::RGBA),
		w(1),
		h(1),
		m_pixels(nullptr)
	{ 
		UInt8* pixels = new UInt8[4]
		{
			p_color.r,
			p_color.g,
			p_color.b,
			p_color.a
		};
		
		m_pixels.reset(pixels);
	}

	Image::~Image()
	{

	}

	//Returns image pixel data
	UInt8* Image::GetPixelData()
	{
		return m_pixels.get();
	}

	//Return pixel data
	const UInt8* Image::GetPixelData() const
	{
		return m_pixels.get();
	}

	//Saves a file into given path
	void Image::WritePNG(const Path& path) const
	{
		stbi_write_png(path.GetPath().c_str(), w, h, static_cast<Int32>(format), m_pixels.get(), w * (static_cast<Int32>(format)));
	}
}