#pragma once

#include <Ace/File.h>
#include <Ace/Color.h>
#include <memory>

namespace ace
{
	enum class PixelFormat
	{
		R = 1,	// Gray
		RG,		// Gray Alpha
		RGB,	// RGB
		RGBA,	// RGBA
	};

	class Image
	{
	public:
		Image(const ace::File &p_file, PixelFormat);
		Image(const ace::Color &p_color);
		Image(UInt8* pixels, int w, int h, PixelFormat);
		
		~Image();

		int w;
		int h;
		PixelFormat format;
	
		UInt8* Pixels();

		void WritePNG(const Path& path) const;
	private:
		std::shared_ptr<UInt8> m_pixels;



	};
}