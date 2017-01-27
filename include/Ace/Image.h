#pragma once

#include <Ace/File.h>
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
		~Image();

		/*	TODO:
			Kuvan luominen väreillä tms		Vaatii color.h
			Kuvan skaalaus					Vaatii jonkin kirjaston??

			TODO???
			Kuvan koko läpinäkyyvyyden säätö??
			Kuvan suora kopiointi ( ja muokkaus)
				Sekä osittainen kopiointi (esim spritesheet tms)
			Kuvan lataaminen toisen kuyvan päälle
			Erilaiset filtterit (kuvan muuttaminen punertavaksi tms)
			Kuvan blurraus
			Kuvan conttrastin muuttaminen
			Kuvan gamman säätö
			Kuvan negatiivisointi
			Kuvaan "melun" lisääminen
			Kuvan pikselöinti
			Kuvan terävöiminen
		*/

		int w;
		int h;
		int comp;  
	
		UInt8* Pixels();
	private:
		std::shared_ptr<UInt8> m_pixels;



	};
}