#pragma once

#include <Ace/File.h>
#include <Ace/Color.h>
#include <memory>

namespace ace
{
	/**
		@brief Pixelformats
	*/
	enum class PixelFormat
	{
		Unknown,
		R,			/**	Gray		*/
		RG,			/**	Gray Alpha	*/
		RGB,		/**	RGB			*/
		RGBA,		/**	RGBA		*/
		Depth,		/** Depth		*/
		Stencil,	/** Stencil		*/
	};

	class Image
	{
	public:

		static Image MissingFile();

		Image(){};

		/**
			@brief Load image data
			@param[in] p_file Image file to load
			@param[in] PixelFormat PixelFormat of the target image file
			@see PixelFormat
		*/
		Image(const ace::File &file, float scale = 1000);

		/**
			@brief Load image with only pixeldata
			@param[in,out] pixels Pixeldata to load
			@param[in] w Width of the image file in pixels
			@param[in] h Height of the image file in pixels
			@param[in] PixelFormat PixelFormat of the target image file
			@see PixelFormat
		*/
		Image(UInt8* pixels, Int32 w, Int32 h, PixelFormat, float scale = 1000);
		
		/**
			@brief Image with colored pixels
			@param[in,out] p_color Color for pixels r, g, b, a
		*/
		Image(const ace::Color &p_color);
		
		~Image();

		int w;
		int h;
		PixelFormat format;
        float scale;
	
		/**
			@brief Returns pointer to pixeldata
		*/
		UInt8* GetPixelData();
		/**
		@brief Returns constant pointer to pixeldata
		*/
		const UInt8* GetPixelData() const;

		/**
			@brief Creates imagefile
			@param path Creates imagefile to the relative path 
		*/
		void WritePNG(const Path& path) const;


	private:
		std::shared_ptr<UInt8> m_pixels;

	};
}