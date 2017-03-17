#include <Ace/Module.h>
#include <Ace/Font.h>
#include <Ace/Image.h>
#include <Ace/Time.h>
#include <Ace/Window.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/Event.h>
#include <Ace/Keyboard.h>
#include <Ace/Sprite.h>
#include <Ace/SpriteManager.h>

#include <allocators>



void update()
{
	ace::Event::Update();

}

void draw()
{

}


int main()
{
	ace::Init();
	ace::Window window("Pong2.0", 1200, 800);
	bool exit = false;

			//Creating stuff
	//Loading images
	ace::Image Ball(ace::File ("/Pong/ball.png"), ace::PixelFormat::RGB);
	ace::Image Enemy(ace::File("/Pong/enemy.png"), ace::PixelFormat::RGB);
	ace::Image Player(ace::File("/Pong/player.png"), ace::PixelFormat::RGB);
	

	//Something is broken
	ace::Texture BallTexture(Ball);
	//ace::Material Ballmaterial();	//Currently empty

	//Vertex and fragment shaders
	ace::Shader fragment, vertex;
	vertex.Load({ "vertex.vert" }, ace::ShaderType::Vertex);
	fragment.Load({ "fragment.frag" }, ace::ShaderType::Fragment);

	//material loading
	ace::Material material(vertex, fragment);
	//material.flags.cullingMode = ace::CullingMode::Both;

	ace::GraphicsDevice::SetTexture(material, BallTexture);

	ace::Sprite BallSprite();

	ace::Font arial(ace::File("arial.ttf"));
	auto Score = arial.BakeTextBox("Score:");


	//GameLoop
	while (!exit)
	{
		update();
		draw();

		//Exit command
		if (ace::Keyboard::GetKey() == ace::KeyCode::F1)
		{
			printf("Exiting game!\n");
			ace::Time::Delay(100);
			exit = true;
		}
	}


}

