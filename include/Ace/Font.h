#pragma once

#include <Ace/File.h>
#include <Ace/Image.h>
#include <Ace/Buffer.h>

#include <memory>
#include <vector>

namespace ace
{
	struct Glyph
	{
		///UInt32 id; 
		///Character
		UInt16 x, y, w, h;
		float xoff, yoff, xadvance;
	};

	class Font
	{
	public:
		///Give file to read font from
		Font(const ace::File& p_file);
		///Destructor
		~Font();

		///BakeFontSheet to image
		///Image int width, int heigth, float pixelheight, int first char (0-255), int num of chars (0-255)
		Image BakeFontSheet(const int& w, const int& h, const float& pixelheight = 32.0f, const int& first_char = 0, const int& num_chars = 255);

		///BakeTextBox image (text image file)
		///char text_to_print, int image_width, int image_heigth, float font_size (32.0F)
		Image BakeTextBox(const char *text_to_print, int w, int h, float lineHeight = 32.0f);
		Image BakeTextBox(const char *text_to_print, float lineHeight = 32.0f);

		///Returns looked glyph
		///const char letter
		Glyph GetGlyph(const UInt32 letter);

		///Buffers text
		void GetTextBuffer(Buffer&, const char* text, float scale = 0.75f, float xPos = -75.0f, float yPos = 0.0f);

		///Font lineheight and space size float
		float m_lineHeight = 42.0f, m_spaceSize = 20.0f;
	private:
		//Image size from baking image to sheet
		int m_w, m_h;

		UInt32 m_start, m_end;

		//Vector where Glyphs are stored
		std::vector<Glyph>ASCII;
		std::vector<Glyph>::iterator it = ASCII.begin();

		//Sharedpointers
		struct FontInfo;
		std::shared_ptr<FontInfo> m_info;
		std::shared_ptr<UInt8> m_buffer;

	};
}