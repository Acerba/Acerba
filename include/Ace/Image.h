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
			Kuvan luominen v�reill� tms		Vaatii color.h
			Kuvan skaalaus					Vaatii jonkin kirjaston??

			TODO???
			Kuvan koko l�pin�kyyvyyden s��t�??
			Kuvan suora kopiointi ( ja muokkaus)
				Sek� osittainen kopiointi (esim spritesheet tms)
			Kuvan lataaminen toisen kuyvan p��lle
			Erilaiset filtterit (kuvan muuttaminen punertavaksi tms)
			Kuvan blurraus
			Kuvan conttrastin muuttaminen
			Kuvan gamman s��t�
			Kuvan negatiivisointi
			Kuvaan "melun" lis��minen
			Kuvan piksel�inti
			Kuvan ter�v�iminen
		*/

		int w;
		int h;
		int comp;  
	
		UInt8* Pixels();
	private:
		std::shared_ptr<UInt8> m_pixels;



	};
}