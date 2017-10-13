// Spritesheet Demo
// Include all necessary modules

#include <Ace/Ace.h>

int main(int, char**)
{
	// Initialize Acerba
	ace::Init();

	// Create window
	ace::Window window("SpriteSheet_demo", 1024u, 768u);

	// Automatic SpriteSheet
	// http://gamedevutils.com
	//ace::SpriteSheet spritesheet("Assets/sheet.json");

	// Manual SpriteSheet
	ace::SpriteSheet spritesheet(ace::Image::MissingFile());
	spritesheet.AddSprite("0", ace::Rect(0, 0, 1, 1)); // Adds Sprite
	spritesheet.AddSprite("1", ace::Rect(1, 0, 1, 1)); // Adds Sprite

	// Create a material
	ace::StandardMaterial material;
	material->diffuse = spritesheet.image;

	ace::Sprite sprite0;
	ace::Sprite sprite1;

	sprite0.SetSprite(spritesheet.GetSprite("0"));	// GetSprite by name.	
	sprite1.SetSprite(spritesheet.GetSprite(1));	// GetSprite by index.

	// Sets tilemap material
	ace::GraphicsDevice::SetMaterial(material);

	ace::Camera camera;

	sprite0.Move(ace::Vector3(-0.6, 0, 0));
	sprite1.Move(ace::Vector3(0.6, 0, 0));

	// While window is open
	while (window) 
	{
		// Clear window
		window.Clear();

		// Update Acerba systems
		ace::Update();

		// Draw sprites
		ace::GraphicsDevice::Draw(sprite0);
		ace::GraphicsDevice::Draw(sprite1);

		// Refresh screen
		window.Present();
	}

	// Shutdown Acerba
	ace::Quit();

	return 0;
}