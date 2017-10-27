#pragma once

#include <Ace/File.h>
#include <Ace/Image.h>
#include <Ace/Buffer.h>

#include <memory>
#include <vector>

namespace ace
{
	/**
		@brief Glyph
	*/
	struct Glyph
	{
		/**
			@param x X-position
			@param y Y-position
			@param w Width
			@param h Height
		*/
		UInt16 x;
		UInt16 y;
		UInt16 w;
		UInt16 h;
		
		/**
			@param xoff Xoffset
			@param yoff Yoffset
			@param xadvance Xadvance
		*/
		float xoff;
		float yoff;
		float xadvance;
	};

	class Font
	{
    public:
        
        /**
            @brief Empty font ctor. Call CreateFont() to fill the font.
            @see CreateFont()
        */
        Font();

		/**
			@brief Font constructor
			@param[in] file File to read
		*/
		Font(const File& file);
		
        /**
            @brief Fill the font object.
            @param[in] file The font file.
            @return True if the operation was successful.
        */
        bool Fill(const File& file);

		/**
			@brief Baking imagefile of the fontsheet from ttf-file
			@param[in] w Width for the fonstsheet image
			@param[in] h Height for the fontsheet image
			@param[in] pixelheight Fontsize in pixels
			@param[in] first_char First charater to save from ttf-file
			@param[in] num_chars Range of characters to bake
			@return Fontsheet imagefile
		*/
		Image BakeFontSheet(UInt32 w, UInt32 h, float pixelheight = 32.0f, Int32 first_char = 0, Int32 num_chars = 255);

		/**
			@brief Bake text into image file
			@param[in] text_to_print Text to save in the image file
			@param[in] w Widht of the image file in pixels
			@param[in] h Height of the image file in pixels
			@param[in] lineHeight Font size in pixels
			@return Text imagefile
		*/
		Image BakeTextBox(const char *text_to_print, Int32 w, Int32 h, float lineHeight = 32.0f);

		/**
			@brief Automated text baking
			@see BakeTextBox
		*/
		Image BakeTextBox(const char *text_to_print, float lineHeight = 32.0f);

		/**
			@return Glyph letter
		*/
		Glyph GetGlyph(const UInt32 letter);

		/**
			@param[in] Buffer Vertexbuffer
			@param[in] text Printable text
			@param[in] scale Text scaling
			@param[in] xPos Text starting position x
			@param[in] yPos Text starting position y
			@return Textbuffer 
		*/
		void GetTextBuffer(Buffer&, const char* text, float scale = 0.75f, float xPos = -75.0f, float yPos = 0.0f);


        /**
        @brief Returns constant raw font data and sets its size to 'size'
        @param[in, out] size Size of the buffer
        @return Constant raw font data
        */
        const UInt8* GetBuffer(UInt32& size) const;
        /**
            @brief Returns raw font data and sets its size to 'size'
            @param[in, out] size Size of the buffer
            @return Raw font data
        */
        UInt8* GetBuffer(UInt32& size);


    private:
        
        struct FontInfo;
        
        // Vector where Glyphs are stored
        std::vector<Glyph> m_ascii;
        std::vector<Glyph>::iterator m_it;
        
        std::shared_ptr<FontInfo> m_info;
        std::shared_ptr<UInt8> m_buffer;
        UInt32 m_bufferSize;
        UInt32 m_start;
        UInt32 m_end;
        // Image size from baking image to sheet
        Int32 m_w;
        Int32 m_h;
        /**
            @brief m_lineHeight Lineheight for printable text
        */
        float m_lineHeight;
        /**
            @brief m_spaceSize spacesize for printable text
        */
        float m_spaceSize;

    };
}