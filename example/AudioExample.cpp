#include <Ace/Module.h>
#include <Ace/Audio.h>
#include <Ace/Time.h>

int main(int, char**)
{
	ace::Init();
	

	ace::AudioClip ExapmleSound;
	bool useAudio = false;
	float volume = 0.5f;
	bool loop = false;

	//Loading Audiosample

	if (ace::File::Exists("ExampleSound.wav"))
		{
			useAudio = true;
			ExapmleSound = ace::AudioClip({"ExampleSound.wav"}, volume, loop);
		}

	// Playing audio once (Loop set to false)
	ace::Audio::PlayAudio(ExapmleSound);

	//setting the volume for all audio samples
	ace::Audio::SetMasterVolume(0.3f);

	//setting the volume of specific audio sample
	ExapmleSound.SetVolume(1.0f);

	
	// Playing a sound on loop until timer is done
	ExapmleSound.SetLoop(true);
	ace::Audio::PlayAudio(ExapmleSound);
	ace::Time::WaitTime wait(200.0f);

	while (wait.IsDone() == true)
	{
		
	}
	ace::Audio::StopAudio(ExapmleSound);


	//TODO Effect examples

	return 0;
}