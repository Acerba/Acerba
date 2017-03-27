#include <Ace/Module.h>
#include <Ace/Font.h>
#include <Ace/Audio.h>
#include <Ace/Image.h>
#include <Ace/Texture.h>


int main(int, char**)
{
	ace::Init();


	// Loading image

	ace::Image exampleImage(0xFFFFFFFFU);

	if (ace::File::Exists("exampleImage.png"))
	{
		exampleImage = ace::Image(ace::File("exampleImage.png"));
	}


	// Writing image

	exampleImage.WritePNG("newExampleImage.png");


	// Loading audio

	ace::AudioClip exampleSound;
	bool useAudio = false;

	if (ace::File::Exists("exampleSound.wav"))
	{
		useAudio = true;
		exampleSound = ace::AudioClip({ "exampleSound.wav" });
	}


	// Loading font and create fontsheet using fontsheet image

	ace::Font arial(ace::File("arial.ttf"));
	ace::Image FontSheet = arial.BakeFontSheet(920, 920, 64, 32, 255);

	ace::Texture fontSheet(FontSheet);



	return 0;
}