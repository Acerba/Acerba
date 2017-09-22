//Input example/demo
#include <Ace\Ace.h>

int main()
{
	//Initialize ACE-engine
	ace::Init();

	//Create window (Name,Widht,Height)
	ace::Window window("Input Demo", 1000, 600);

	//Create colors
	ace::Color Red(255, 0, 0, 255);
	ace::Color Green(0, 255, 0, 255);
	ace::Color Blue(0, 0, 255, 255);
	ace::Color White(255, 255, 255, 255);

	//Loop that goes on forever while window is open
	while (window)
	{
		//Engine update
		ace::Update();

		//Close window by pressing F1
		if (ace::Keyboard::GetKey() == ace::KeyCode::F1)
		{
			ace::Time::Delay(100);
			window.Close();
		}

		//Change window color by pressing A
		if (ace::Keyboard::GetKey() == ace::KeyCode::A)
		{
			window.SetClearColor(Red); 
		}

		//Change window color by pressing W
		if (ace::Keyboard::GetKey() == ace::KeyCode::W)
		{
			window.SetClearColor(Green);
		}

		//Change window color by pressing S
		if (ace::Keyboard::GetKey() == ace::KeyCode::S)
		{
			window.SetClearColor(Blue);
		}

		//Change window color by pressing D
		if (ace::Keyboard::GetKey() == ace::KeyCode::D)
		{
			window.SetClearColor(White);
		}

		//Update everything that have been modified to window
		window.Present();
	}

}