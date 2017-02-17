// stb_truetype.h lis‰‰minen
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <Ace/Font.h>

namespace ace
{
	struct Font::FontInfo
	{
		stbtt_fontinfo font;
	};

	Font::Font(const ace::File& p_file) :
		m_info(std::make_shared<Font::FontInfo>()) //Eetu "muutti" rivin t.Vepe
	{
		//Font is placed inside buffer
		m_buffer = p_file.ReadAll();

		//Loads font file from a memory buffer
		stbtt_InitFont(&m_info->font, m_buffer.get(), stbtt_GetFontOffsetForIndex(m_buffer.get(), 0));

	}

	Font::~Font()
	{

	}

	///BakeFontSheet to image
	///Image int width, int heigth, float pixelheight int first char, int num of chars (0-255)
	ace::Image ace::Font::BakeFontSheet(const int& w, const int& h, const float& pixelheight, const int& first_char, const int& num_chars)
	{

		stbtt_bakedchar cdata[255]; // ASCII 32..126 is 95 glyphs
		UInt8* bitmap = new UInt8[w*h];

		int nW, nH;
		stbtt_GetCodepointBitmap(&m_info->font, 0, stbtt_ScaleForPixelHeight(&m_info->font, 16), 'a', &nW, &nH, 0, 0);

		//-- bake a font to a bitmap for use as texture
		stbtt_BakeFontBitmap(m_buffer.get(), 0, pixelheight, bitmap, w, h, first_char, num_chars, cdata);

		Image image(bitmap, w, h, ace::PixelFormat::R);

		return image;
	}
	
	///Baking text box with given width and height
	ace::Image ace::Font::BakeTextBox(const char *text_to_print, int w, int h, float lineHeight)
	{
		int totalSize = w * h;
		UInt8* bitmap = new UInt8[totalSize];
		std::fill(bitmap, bitmap + totalSize, 0);

		float scale = stbtt_ScaleForPixelHeight(&m_info->font, lineHeight);
		float x = 0;
		int ascent, descent, lineGap;

		stbtt_GetFontVMetrics(&m_info->font, &ascent, &descent, &lineGap);
		ascent *= scale;
		descent *= scale;

		// Row that text will be written on
		int writableRow = 0;

		int length = strlen(text_to_print);
		for (int i = 0; i < length; ++i)
		{
			int character = text_to_print[i];
			int nextCharacter = text_to_print[i + 1];

			// Checking characters if they contain 
			if (character < 0)
			{
				character = character + 256;
			}
			if (nextCharacter < 0)
			{
				nextCharacter = nextCharacter + 256;
			}

			float x_shift = x - (float)floor(x);

			/* get bounding box for character (may be offset to account for chars that dip above or below the line */
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBoxSubpixel(&m_info->font, character, scale, scale, x_shift, 0, &c_x1, &c_y1, &c_x2, &c_y2);

			/* compute y (different characters have different heights */
			int y = ascent + c_y1;

			// Chacking if character is new line command
			const char newLine = '\n';
			if (character == newLine)
			{
				writableRow = writableRow + 1;
				x = 0;
				continue;
			}

			/* render character (stride and offset is important here) */
			int byteOffset = (int)x + c_x1 + (((writableRow * lineHeight) + y) * w);
			stbtt_MakeCodepointBitmapSubpixel(&m_info->font, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, w, scale, scale, x_shift, 0, character);

			/* how wide is this character */
			int ax;
			stbtt_GetCodepointHMetrics(&m_info->font, character, &ax, 0);
			x += ax * scale;

			/* add kerning */
			int kern;
			if (nextCharacter)
			{
				kern = stbtt_GetCodepointKernAdvance(&m_info->font, character, nextCharacter);
				x += kern * scale;
			}
		}
		return Image(bitmap, w, h, PixelFormat::R);
	}

	///Preparing image for baking with minimum empty image space
	ace::Image ace::Font::BakeTextBox(const char *text_to_print, float lineHeight)
	{
		int w = 0;
		int h = lineHeight;
		int writableRow = 0;

		float scale = stbtt_ScaleForPixelHeight(&m_info->font, lineHeight);
		float x = 0;
		int ascent, descent, lineGap;

		stbtt_GetFontVMetrics(&m_info->font, &ascent, &descent, &lineGap);
		ascent *= scale;
		descent *= scale;

		int length = strlen(text_to_print);
		for (int i = 0; i < length; ++i)
		{
			int character = text_to_print[i];
			int nextCharacter = text_to_print[i + 1];

			// Checking characters if they contain 
			if (character < 0)
			{
				character = character + 256;
			}
			if (nextCharacter < 0)
			{
				nextCharacter = nextCharacter + 256;
			}

			float x_shift = x - (float)floor(x);

			/* get bounding box for character (may be offset to account for chars that dip above or below the line */
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBoxSubpixel(&m_info->font, character, scale, scale, x_shift, 0, &c_x1, &c_y1, &c_x2, &c_y2);

			/* compute y (different characters have different heights */
			int y = ascent + c_y1;

			// Chacking if character is new line command
			const char newLine = '\n';
			if (character == newLine)
			{
				writableRow = writableRow + 1;
				x = 0;
			}

			/* how wide is this character */
			int ax;
			stbtt_GetCodepointHMetrics(&m_info->font, character, &ax, 0);
			x += ax * scale;

			/* add kerning */
			int kern;
			if (nextCharacter)
			{
				kern = stbtt_GetCodepointKernAdvance(&m_info->font, character, nextCharacter);
				x += kern * scale;
				if (w < x)
				{
					w = x;
				}
			}
		}

		h = (writableRow + 1) * lineHeight;

		return BakeTextBox(text_to_print, w, h, lineHeight);
	}

	///Saving font character data
	void Font::SaveAllFonts(int x, int y, const int w, const int h)
	{
		/*
			eli x ja y ovat koordinaatit kullekkin kirjaimelle, 
			w ja h ovat kirjaimen koon tallentaminen??
		*/
		
	}
}