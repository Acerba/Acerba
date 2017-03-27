#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <Ace/Image.h>

namespace ace
{

	Image Image::MissingFile()
	{
		Int32 w = 4;
		Int32 h = 4;

		UInt8* pixels = new UInt8[w*h*16];
		
		pixels[0 + 0 * 4] = 255;
		pixels[1 + 0 * 4] = 255;
		pixels[2 + 0 * 4] = 255;
		pixels[3 + 0 * 4] = 255;
				   
		pixels[0 + 1 * 4] = 186;
		pixels[1 + 1 * 4] = 85;
		pixels[2 + 1 * 4] = 211;
		pixels[3 + 1 * 4] = 255;
				   
		pixels[0 + 2 * 4] = 255;
		pixels[1 + 2 * 4] = 255;
		pixels[2 + 2 * 4] = 255;
		pixels[3 + 2 * 4] = 255;
				   
		pixels[0 + 3 * 4] = 186;
		pixels[1 + 3 * 4] = 85;
		pixels[2 + 3 * 4] = 211;
		pixels[3 + 3 * 4] = 255;

		pixels[0 + 4 * 4] = 186;
		pixels[1 + 4 * 4] = 85;
		pixels[2 + 4 * 4] = 211;
		pixels[3 + 4 * 4] = 255;

		pixels[0 + 5 * 4] = 255;
		pixels[1 + 5 * 4] = 255;
		pixels[2 + 5 * 4] = 255;
		pixels[3 + 5 * 4] = 255;

		pixels[0 + 6 * 4] = 186;
		pixels[1 + 6 * 4] = 85;
		pixels[2 + 6 * 4] = 211;
		pixels[3 + 6 * 4] = 255;

		pixels[0 + 7 * 4] = 255;
		pixels[1 + 7 * 4] = 255;
		pixels[2 + 7 * 4] = 255;
		pixels[3 + 7 * 4] = 255;

		pixels[0 + 8 * 4] = 255;
		pixels[1 + 8 * 4] = 255;
		pixels[2 + 8 * 4] = 255;
		pixels[3 + 8 * 4] = 255;

		pixels[0 + 9 * 4] = 186;
		pixels[1 + 9 * 4] = 85;
		pixels[2 + 9 * 4] = 211;
		pixels[3 + 9 * 4] = 255;

		pixels[0 + 10 * 4] = 255;
		pixels[1 + 10 * 4] = 255;
		pixels[2 + 10 * 4] = 255;
		pixels[3 + 10 * 4] = 255;

		pixels[0 + 11 * 4] = 186;
		pixels[1 + 11 * 4] = 85;
		pixels[2 + 11 * 4] = 211;
		pixels[3 + 11 * 4] = 255;

		pixels[0 + 12 * 4] = 186;
		pixels[1 + 12 * 4] = 85;
		pixels[2 + 12 * 4] = 211;
		pixels[3 + 12 * 4] = 255;

		pixels[0 + 13 * 4] = 255;
		pixels[1 + 13 * 4] = 255;
		pixels[2 + 13 * 4] = 255;
		pixels[3 + 13 * 4] = 255;

		pixels[0 + 14 * 4] = 186;
		pixels[1 + 14 * 4] = 85;
		pixels[2 + 14 * 4] = 211;
		pixels[3 + 14 * 4] = 255;

		pixels[0 + 15 * 4] = 255;
		pixels[1 + 15 * 4] = 255;
		pixels[2 + 15 * 4] = 255;
		pixels[3 + 15 * 4] = 255;

		return Image(pixels, w, h, PixelFormat::RGBA);
	}

	//Loads image from file
	//Image PixelFormat: Gray, Gray Alpha, RGB, RGBA
	Image::Image(const ace::File& p_file)
	{
		if (!p_file)
		{
			*this = MissingFile();
			return;
		}

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