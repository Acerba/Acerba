#pragma once

#include <Ace/File.h>
#include <Ace/Color.h>
#include <memory>

namespace ace
{
	enum class PixelFormat
	{
		R,		// Gray
		RG,		// Gray Alpha
		RGB,	// RGB
		RGBA,	// RGBA
	};

	class Image
	{
	public:
		Image(const ace::File &p_file, PixelFormat);
		Image(const ace::Color &p_color);
		~Image();

		int w;
		int h;
		int comp;  
	
		UInt8* Pixels();
	private:
		std::shared_ptr<UInt8> m_pixels;



	};
}