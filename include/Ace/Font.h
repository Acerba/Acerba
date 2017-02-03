#pragma once

#include <Ace/File.h>
#include <Ace/Image.h>

#include <memory>

namespace ace
{
	class Font
	{
	public:
		//Give file to read font from
		Font(const ace::File& p_file);
		~Font();

		
		Image BakeFontSheet(const int w, const int h);

	private:

		struct FontInfo;
		std::shared_ptr<FontInfo> m_info;
		std::shared_ptr<UInt8> m_buffer;

	};
}