//Audio demo
#include <Ace\Ace.h>

int main(int, char**)
{
	//Initialize ACE-engine
	ace::Init();

	//Create window (Name, Width, Height)
	ace::Window window("Title", 800, 600);

	//Create AudioClip
	ace::AudioClip ExampleSound;
	ace::Path AudioClip = "assets/blop.ogg";
	//Bool for using audio (optional)
	bool useAudio = false;
	//Loading Audiosample to AudioClip
	if (ace::File::Exists(AudioClip))
	{
		useAudio = true;
		ExampleSound = ace::AudioClip({ AudioClip });
	}

	//Loop that goes on forever while window is open
	while (window)
	{
		//Engine update
		ace::Update();

		//Play sound when A is pressed
		if (ace::Keyboard::KeyPressed(ace::KeyCode::A))
		{
			if (useAudio)
			{
				ace::Audio::PlayAudio(ExampleSound);
				ace::Time::Delay(100);
			}
		}

	}
}