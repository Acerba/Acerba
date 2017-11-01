//Audio demo
#include <Ace/Ace.h>

int main(int, char**)
{
	//Initialize ACE-engine
	ace::Init();

	//Create window (Name, Width, Height)
	ace::Window window("Title", 800, 600);

	//Create empty AudioClip
	ace::AudioClip ExampleSound;
	
	//Create path to the audio file
	const ace::Path clip = "assets/blop.ogg";
	//Bool for using audio (optional)
	bool useAudio = false;
	//Loading Audiosample to AudioClip
	if (ace::File::Exists(clip))
	{
		useAudio = true;
		ExampleSound = ace::AudioClip(ace::File(clip));
	}

	//Loop that goes on forever while window is open
	while (window)
	{
		//Engine update
		ace::Update();

		//Play sound when A is pressed
		if (ace::Keyboard::GetKey("A"))
		{
			if (useAudio)
			{
				ace::Audio::PlayAudio(ExampleSound);
				ace::Time::Delay(100);
			}
		}
	}
	ace::Quit();
	return 0;
}