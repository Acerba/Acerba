#include <Ace/Ace.h>
#include <iostream>
#include <Ace/Debugger.h>
// This demo uses direct calls to GraphicsDevice to draw.

int main(int, char**)
{
	ace::Init();
	ace::Window window("GraphicsDemo", 800, 600);

	ace::GraphicsDevice::Enable(true, ace::Features::Blend);
	ace::Sprite exampleSprite;
	ace::Camera camera;
	ace::Scene scene;
	scene.GetRoot().AddChild(camera.GetEntity());
	while (window)
	{
		ace::Vector3 position(camera.GetPosition());
		window.Clear();

		ace::Update();

		camera.Apply();

		camera.Update();

		if (ace::Keyboard::GetKey("W"))
		{
			position.y += 0.01f;
		}

		if (ace::Keyboard::GetKey("A"))
		{
			position.x -= 0.01f;
		}

		if (ace::Keyboard::GetKey("S"))
		{
			position.y -= 0.01f;
		}

		if (ace::Keyboard::GetKey("D"))
		{
			position.x += 0.01f;
		}

		camera.SetPosition(position);
		ace::LogDebug(ace::Mouse::GetWorldPosition(camera), "");

		ace::GraphicsDevice::Draw(exampleSprite);
		scene.Update();
		scene.Draw(camera);
		window.Present();
	}

	// Shutdown Acerba
	ace::Quit();

	return 0;
}
