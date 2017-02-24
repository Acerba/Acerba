// stb_truetype.h lisääminen
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <Ace/Font.h>
#include <Ace/GraphicsDevice.h>

namespace ace
{
	void CreateGlyphs(std::vector<Glyph>& glyphs, stbtt_bakedchar* cdata, UInt32 size, UInt32 first)
	{

		for (int i = 0; i < size; i++)
		{
			Glyph glyph;
			
			glyph.h = cdata[i].y1 - cdata[i].y0;
			glyph.w = cdata[i].x1 - cdata[i].x0;

			glyph.x = cdata[i].x0;
			glyph.y = cdata[i].y0;
			
			glyph.xoff = cdata[i].xoff;
			glyph.yoff = cdata[i].yoff;

			glyph.xadvance = cdata[i].xadvance;

			glyphs.push_back(glyph);
		}
	}

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
	Image Font::BakeFontSheet(const int& w, const int& h, const float& pixelheight, const int& first_char, const int& num_chars)
	{

		stbtt_bakedchar cdata[255]; // ASCII 32..126 is 95 glyphs
		UInt8* bitmap = new UInt8[w*h];

		int nW, nH;
		stbtt_GetCodepointBitmap(&m_info->font, 0, stbtt_ScaleForPixelHeight(&m_info->font, 16), 'a', &nW, &nH, 0, 0);
		
		//-- bake a font to a bitmap for use as texture
		stbtt_BakeFontBitmap(m_buffer.get(), 0, pixelheight, bitmap, w, h, first_char, num_chars, cdata);

		//Saves image size
		m_w = w;
		m_h = h;

		//Saves where glyps start and end
		m_start = first_char;
		m_end = num_chars;

		//Adds glyps into vector for later use
		CreateGlyphs(ASCII, cdata, 255, first_char);

		Image image(bitmap, w, h, ace::PixelFormat::R);


		return image;
	}
	
	///Baking text box with given width and height
	Image Font::BakeTextBox(const char *text_to_print, int w, int h, float lineHeight)
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
	Image Font::BakeTextBox(const char *text_to_print, float lineHeight)
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

			// Checking characters if they contain  Å, å, Ä, ä, Ö, or ö
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

	//Returns glyph
	Glyph Font::GetGlyph(const char letter)
	{
		UInt32 index = letter;
		index -= m_start;

		if (index < 0 || index > m_end)
		{
			return Glyph();
		}

		return ASCII[index];
	}


	void Font::GetTextBuffer(Buffer& buffer, const char* text)
	{
		UInt32 len = strlen(text);

		if (len < 0)
		{
			return;
		}

		Vertex* vertex = new Vertex[6 * len];


		/*
		Elikkäs:
			jokainen kirjain (glyph) sisältää 6 verteksiä (2 kolmiota)
			jokainen vertex tulee määrittää itse oikeanlaiseksi, antaen
			värin paikan ja uv:n (joista uv on kinnkisin)
			koska se tarvii glyphin x,y:n ja se tulee jakaa kuvalla josta se on otettu

			väri: ? öööh

			paikka: x ja y apuna,
				vasen ylä -> vasen ala		oikea ylä -> oikea ala
				vasen ala -> oikea ala		oikea ala -> vasen ylä
				oikea ala -> vasen ylä		vasen ylä -> oikea ylä
			
			uv: ?? hyöööögh
				x ja y apuna? , kuva josta glyph otettiin, jaetaan x ja y:llä, tulokseski
				tuleva määrä on uv?
		*/


		for (int i = 0; i < len; ++i)
		{
			Glyph g = GetGlyph(text[i]);

			/*	Verteksin piseet tulisi antaa Vector4 muodossa, jolloin tulisi 
			tietää x,y,(z),(z2), mutta 2D sisältää vain  X:n ja Y:n				*/
			////Adding vertex positions
			//vertex[6 * i + 0].position = g.x + g.y;
			//vertex[6 * i + 1].position = g.x + g.h;
			//vertex[6 * i + 2].position = g.w + g.h;
			//
			//vertex[6 * i + 3].position = g.w + g.y;
			//vertex[6 * i + 4].position = g.w + g.h;
			//vertex[6 * i + 5].position = g.x + g.y;


			/*	värit tulisi ottaa siten että fontti ei mitenkään muuttuisi tai
				sitten olisi täysin valkoinen?									*/
			//Adding vertex colors	r,g,b,a
			vertex[6 * i + 0].color = Color32(0, 0, 0, 1);
			vertex[6 * i + 1].color = Color32(0, 0, 0, 1);
			vertex[6 * i + 2].color = Color32(0, 0, 0, 1);

			vertex[6 * i + 3].color = Color32(0, 0, 0, 1);
			vertex[6 * i + 4].color = Color32(0, 0, 0, 1);
			vertex[6 * i + 5].color = Color32(0, 0, 0, 1);

			/*	UV tulisi ottaa kirjaimen koon mukaan, kirjaimen alue tulisi jakaa
				tasaisesti omiin "pikseleihinsä"(?)									*/
			//Adding vertex UV	x & y
			vertex[6 * i + 0].uv = Vector2(g.w / m_w, g.h / m_h);
			vertex[6 * i + 1].uv = Vector2(g.w / m_w, g.h / m_h);
			vertex[6 * i + 2].uv = Vector2(g.w / m_w, g.h / m_h);

			vertex[6 * i + 3].uv = Vector2(g.w / m_w, g.h / m_h);
			vertex[6 * i + 4].uv = Vector2(g.w / m_w, g.h / m_h);
			vertex[6 * i + 5].uv = Vector2(g.w / m_w, g.h / m_h);

		}

		GraphicsDevice::BufferData(buffer, len, vertex);
		delete[] vertex;
	}
}