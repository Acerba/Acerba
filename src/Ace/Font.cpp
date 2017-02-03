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


	//File reads the ttf file where font is stored
	Font::Font(const ace::File& p_file) : m_info(new Font::FontInfo())
	{
		//Font is placed inside buffer
		m_buffer = p_file.ReadAll();
		
		BakeFontSheet(512,512);
	}

	Font::~Font()
	{

	}

	ace::Image ace::Font::BakeFontSheet(const int w, const int h)
	{
		//Loads font file from a memory buffer
		stbtt_InitFont(&m_info->font, m_buffer.get(), stbtt_GetFontOffsetForIndex(m_buffer.get(), 0));
		
		stbtt_bakedchar cdata[255]; // ASCII 32..126 is 95 glyphs
		UInt8* bitmap = new UInt8[w*h];
		
		int nW, nH;
		stbtt_GetCodepointBitmap(&m_info->font, 0,stbtt_ScaleForPixelHeight(&m_info->font,16), 'a', &nW,&nH,0,0);

		//-- bake a font to a bitmap for use as texture
		stbtt_BakeFontBitmap(m_buffer.get(), 0, 32, bitmap, w, h, 0, 255, cdata);
		
		Image image(bitmap, w, h, ace::PixelFormat::R);
		
		/*	Problems:
		-How to get right size (w,h) to make bitmap for imagfile,
		-How to get right size for font (w & h) in pixels, [testing it out maybe??]
		-
		*/

		//Only for testing
		image.Write("TestFontImage.png");

		return image;
	}


}