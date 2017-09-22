# Main Page


## Example


```{.cpp}

#include <Ace/Ace.h>

int main(int argc, char** args)
{
	ace::Init(argc, args);

	ace::Window window("Main", 512, 512);
	window.SetClearColor(ace::Color(255, 127, 127));
	
	while(window)
	{
		window.Clear();
		
		ace::Update();
		
		// Logic & Rendering
		
		window.Present();
	}
	
	ace::Quit();
	
	return 0;
}

```