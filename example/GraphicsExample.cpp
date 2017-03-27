#include <Ace/Module.h>
#include <Ace/File.h>
#include <Ace/Image.h>
#include <Ace/Font.h>
#include <Ace/Sprite.h>
#include <Ace/Texture.h>
#include <Ace/Shader.h>
#include <Ace/Material.h>
#include <Ace/Window.h>
#include <Ace/GraphicsDevice.h>


void draw(ace::Material& material, ace::Sprite& sprite, ace::Texture& texture, const ace::Vector2& position, const ace::Vector2& scale)
{
	material.Uniform("position", position);
	material.Uniform("scale", scale);

	ace::GraphicsDevice::SetTexture(material, texture, "Texture", 0);
	ace::GraphicsDevice::Draw(material, sprite);
}

int main(int, char**)
{
	ace::Init();

	// Loading images

	ace::Image exampleImage(0xFFFFFFFFU);

	if (ace::File::Exists("ExampleImage.png"))
		{
			exampleImage = ace::Image(ace::File("ExampleImage.png"));
		}

	//Loading textures

	ace::Texture exampleTexture(exampleImage);


	// Vertex and fragment shaders

	ace::Shader fragment, vertex;
	vertex.Load({ "vertex.vert" }, ace::ShaderType::Vertex);
	fragment.Load({ "fragment.frag" }, ace::ShaderType::Fragment);

	// Material loading

	ace::Material material(vertex, fragment);
	material.flags.cullingMode = ace::CullingMode::Both;


	//creating a sprite

	ace::Sprite exampleSprite;
	

	// Position for sprite
	ace::Vector2 position = ace::Vector2{ 0.0f, 0.0f };

	// drawing sprite

	draw(material, exampleSprite, exampleTexture, position, { 0.2f, 2.0f });


	// Loading text

	ace::Font arial(ace::File("arial.ttf"));
	ace::Image FontSheet = arial.BakeFontSheet(920, 920, 64, 32, 255);

	
	//TODO add vertex and fragment shaders, add example for drawing text
			

	return 0;
}