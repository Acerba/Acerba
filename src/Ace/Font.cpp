// stb_truetype.h lis‰‰minen
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <Ace/Font.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/Math.h>

namespace ace
{
    void CreateGlyphs(std::vector<Glyph>& glyphs, stbtt_bakedchar* cdata, UInt32 size, UInt32 first)
    {

        for (UInt32 i = 0u; i < size; i++)
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

    Font::Font(const File& p_file) :
        m_info(std::make_shared<Font::FontInfo>()) //Eetu "muutti" rivin t.Vepe
    {
        //Font is placed inside buffer
        m_buffer = p_file.ReadAll();

        m_bufferSize = p_file.Size();

        //Loads font file from a memory buffer
        stbtt_InitFont(&m_info->font, m_buffer.get(), stbtt_GetFontOffsetForIndex(m_buffer.get(), 0));

    }

    Font::~Font()
    {

    }

    ///BakeFontSheet to image
    ///Image int width, int heigth, float pixelheight int first char, int num of chars (0-255)
    Image Font::BakeFontSheet(UInt32 w, UInt32 h, float pixelheight, Int32 first_char, Int32 num_chars)
    {

        stbtt_bakedchar cdata[255]; // ASCII 32..126 is 95 glyphs
        UInt8* bitmap = new UInt8[w*h];

        Int32 nW = 0, nH = 0;

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

        static const PixelFormat format = PixelFormat::RGBA;
        static const UInt8 channels = static_cast<UInt8>(format);

        // Adds RGBA channels

        UInt8* rgba = new UInt8[w*h*channels];

        for (UInt32 i = 0u, j = 0u; i < w*h; ++i, j += channels)
        {
            rgba[j + 0u] = bitmap[i];
            rgba[j + 1u] = bitmap[i];
            rgba[j + 2u] = bitmap[i];
            rgba[j + 3u] = bitmap[i];
        }

        delete[] bitmap;

        return Image(rgba, w, h, format);
    }

    ///Baking text box with given width and height
    Image Font::BakeTextBox(const char *text_to_print, Int32 w, Int32 h, float lineHeight)
    {
        UInt8* bitmap = new UInt8[w * h];
        std::fill(bitmap, bitmap + (w * h), 0);

        float scale = stbtt_ScaleForPixelHeight(&m_info->font, lineHeight);
        float x = 0.f;
        Int32 ascent = 0, descent = 0, lineGap = 0;

        stbtt_GetFontVMetrics(&m_info->font, &ascent, &descent, &lineGap);
        ascent *= scale;
        descent *= scale;

        // Row that text will be written on
        Int32 writableRow = 0;

        UInt32 length = strlen(text_to_print);
        for (UInt32 i = 0u; i < length; ++i)
        {
            Int32 character = text_to_print[i];
            Int32 nextCharacter = text_to_print[i + 1];

            // Checking characters if they contain 
            if (character < 0)
            {
                character += 256;
            }
            if (nextCharacter < 0)
            {
                nextCharacter += 256;
            }

            const float x_shift = x - math::Floor(x);

            /* get bounding box for character (may be offset to account for chars that dip above or below the line */
            Int32 c_x1 = 0, c_y1 = 0, c_x2 = 0, c_y2 = 0;
            stbtt_GetCodepointBitmapBoxSubpixel(&m_info->font, character, scale, scale, x_shift, 0, &c_x1, &c_y1, &c_x2, &c_y2);

            /* compute y (different characters have different heights */
            // const Int32 y = ascent + c_y1;

            // Chacking if character is new line command
            if (character == '\n')
            {
                ++writableRow;
                x = 0.f;
                continue;
            }

            /* render character (stride and offset is important here) */
            Int32 byteOffset = (Int32)x + c_x1 + (((writableRow * lineHeight) + ascent + c_y1) * w);
            stbtt_MakeCodepointBitmapSubpixel(&m_info->font, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, w, scale, scale, x_shift, 0, character);

            /* how wide is this character */
            Int32 ax = 0;
            stbtt_GetCodepointHMetrics(&m_info->font, character, &ax, 0);
            x += ax * scale;

            /* add kerning */
            if (nextCharacter)
            {
                x += (scale * stbtt_GetCodepointKernAdvance(&m_info->font, character, nextCharacter));
            }
        }
        return Image(bitmap, w, h, PixelFormat::R);
    }

    ///Preparing image for baking with minimum empty image space
    Image Font::BakeTextBox(const char *text_to_print, float lineHeight)
    {
        const float scale = stbtt_ScaleForPixelHeight(&m_info->font, lineHeight);
        float x = 0.f;
        Int32 ascent = 0, descent = 0, lineGap = 0;

        stbtt_GetFontVMetrics(&m_info->font, &ascent, &descent, &lineGap);
        ascent *= scale;
        descent *= scale;

        UInt32 length = strlen(text_to_print);
        UInt32 writableRow = 0u;
        Int32 w = 0;

        for (UInt32 i = 0u; i < length; ++i)
        {
            Int32 character = text_to_print[i];
            Int32 nextCharacter = text_to_print[i + 1];

            // Checking characters if they contain  ≈, Â, ƒ, ‰, ÷, or ˆ
            if (character < 0)
            {
                character += 256;
            }
            if (nextCharacter < 0)
            {
                nextCharacter += 256;
            }

            float x_shift = x - math::Floor(x);

            /* get bounding box for character (may be offset to account for chars that dip above or below the line */
            Int32 c_x1 = 0, c_y1 = 0, c_x2 = 0, c_y2 = 0;
            stbtt_GetCodepointBitmapBoxSubpixel(&m_info->font, character, scale, scale, x_shift, 0, &c_x1, &c_y1, &c_x2, &c_y2);

            /* compute y (different characters have different heights */
            Int32 y = ascent + c_y1;

            // Chacking if character is new line command
            if (character == '\n')
            {
                ++writableRow;
                x = 0.f;
            }

            /* how wide is this character */
            Int32 ax = 0;
            stbtt_GetCodepointHMetrics(&m_info->font, character, &ax, 0);
            x += ax * scale;

            /* add kerning */
            if (nextCharacter)
            {
                x += (scale * stbtt_GetCodepointKernAdvance(&m_info->font, character, nextCharacter));
                if (w < x)
                {
                    w = x;
                }
            }
        }
        const Int32 h = (writableRow + 1) * lineHeight;
        return BakeTextBox(text_to_print, w, h, lineHeight);
    }

    //Returns glyph
    Glyph Font::GetGlyph(const UInt32 letter)
    {
        UInt32 index = letter;
        index -= m_start;

        if (index < 0 || index > m_end)
        {
            return Glyph();
        }

        return ASCII[index];
    }

    //Buffers text
    void Font::GetTextBuffer(Buffer& buffer, const char* text, float scale, float xPos, float yPos)
    {
        const UInt32 len = strlen(text);
        //First position for X
        float XPos = xPos;

        if (len < 0)
        {
            return;
        }

        Vertex* vertex = new Vertex[6 * len];

        for (UInt32 i = 0u; i < len; ++i)
        {
            // Checking if there is space 
            if (text[i] == ' ')
            {
                xPos += m_spaceSize * scale;
                continue;
            }

            // Checking if there is endline 
            if (text[i] == '\n')
            {
                yPos -= (m_lineHeight * scale);
                xPos = XPos;
                continue;
            }

            // Checking characters if they contain  ≈, Â, ƒ, ‰, ÷, or ˆ
            Int32 character = text[i];
            if (character < 0)
            {
                character = character + 256;
            }

            Glyph g = GetGlyph(character);

            //TODO: remove following if the next one works
            // auto uv0 = Vector2((float)(g.x + g.w) / m_w, (float)(g.y + 0) / m_h);
            // auto uv1 = Vector2((float)(g.x + g.w) / m_w, (float)(g.y + g.h) / m_h);
            // auto uv2 = Vector2((float)(g.x + 0) / m_w, (float)(g.y + g.h) / m_h);
            // auto uv3 = Vector2((float)(g.x + g.w) / m_w, (float)(g.y + 0) / m_h);
            // auto uv4 = Vector2((float)(g.x + 0) / m_w, (float)(g.y + 0) / m_h);
            // auto uv5 = Vector2((float)(g.x + 0) / m_w, (float)(g.y + g.h) / m_h);
            // vertex[6 * i + 0].uv = uv5;
            // vertex[6 * i + 2].uv = uv4;
            // vertex[6 * i + 1].uv = uv3;
            // vertex[6 * i + 3].uv = uv2;
            // vertex[6 * i + 4].uv = uv1;
            // vertex[6 * i + 5].uv = uv0;

            //Adding vertex UV
            vertex[6 * i + 0].uv = Vector2(static_cast<float>((g.x + 0) / m_w),     static_cast<float>((g.y + g.h) / m_h));
            vertex[6 * i + 2].uv = Vector2(static_cast<float>((g.x + 0) / m_w),     static_cast<float>((g.y + 0) / m_h));
            vertex[6 * i + 1].uv = Vector2(static_cast<float>((g.x + g.w) / m_w),   static_cast<float>((g.y + 0) / m_h));
            vertex[6 * i + 3].uv = Vector2(static_cast<float>((g.x + 0) / m_w),     static_cast<float>((g.y + g.h) / m_h));
            vertex[6 * i + 4].uv = Vector2(static_cast<float>((g.x + g.w) / m_w),   static_cast<float>((g.y + g.h) / m_h));
            vertex[6 * i + 5].uv = Vector2(static_cast<float>((g.x + g.w) / m_w),   static_cast<float>((g.y + 0) / m_h));

            g.w *= scale;
            g.h *= scale;
            g.xoff *= scale;
            g.yoff *= scale;
            g.xadvance *= scale;

            //Adding vertex positions

            const float y = yPos;
            yPos -= (g.h + g.yoff);

            vertex[6 * i + 0].position = Vector4(xPos, yPos, 0, 0);
            vertex[6 * i + 2].position = Vector4(xPos, yPos + g.h, 0, 0);
            vertex[6 * i + 1].position = Vector4(xPos + g.w, yPos + g.h, 0, 0);

            vertex[6 * i + 3].position = Vector4(xPos, yPos, 0, 0);
            vertex[6 * i + 4].position = Vector4(xPos + g.w, yPos, 0, 0);
            vertex[6 * i + 5].position = Vector4(xPos + g.w, yPos + g.h, 0, 0);

            yPos = y;
            xPos += g.xoff + g.w;

            //Adding vertex colors	r,g,b,a
            vertex[6 * i + 0].color = Color32(1, 1, 1, 1);
            vertex[6 * i + 1].color = Color32(1, 1, 1, 1);
            vertex[6 * i + 2].color = Color32(1, 1, 1, 1);
            vertex[6 * i + 3].color = Color32(1, 1, 1, 1);
            vertex[6 * i + 5].color = Color32(1, 1, 1, 1);
            vertex[6 * i + 4].color = Color32(1, 1, 1, 1);
        }

        GraphicsDevice::BufferData(buffer, len * 6, vertex);
        delete[] vertex;
    }


    const UInt8* Font::GetBuffer(UInt32& size) const
    {
        size = m_bufferSize;
        return m_buffer.get();
    }


    UInt8* Font::GetBuffer(UInt32& size)
    {
        size = m_bufferSize;
        return m_buffer.get();
    }

}