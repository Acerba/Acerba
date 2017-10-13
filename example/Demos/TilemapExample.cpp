// Tilemap Demo
// Include all necessary modules
// REQUIRES: map.tmx file!

#include <Ace/Ace.h>

int main(int, char**) 
{
	// Initialize Acerba
	ace::Init();

	// Create window
	ace::Window window("Tilemap_demo", 1024u, 768u);


	// REQUIRES: map.tmx file!
	// Loads tilemap
	ace::Tilemap tilemap("Assets/map.tmx", 1.0f, ace::Vector3(0.5,0.5,0));

	// Create a material
	ace::StandardMaterial tilemapMaterial;

	// Sets tilemap texture
	tilemapMaterial->diffuse = tilemap.tileset;
	
	// Sets tilemap material
	ace::GraphicsDevice::SetMaterial(tilemapMaterial);

	ace::Camera camera;
	camera.Ortho(10, 10, 0, 10);

	// Entity based Tilemap rendering:
	//ace::Scene world;
	//ace::Entity tilemapEntity;	
	//tilemapEntity.AddComponent<ace::Material>(mat);
	//tilemapEntity.AddComponent<ace::Drawable*>(new ace::Tilemap(map)); // Requires: ace::Drawable*
	//world.GetRoot().AddChild(tilemapEntity);

	// While window is open
	while (window) {

		// Clear window
		window.Clear();

		// Update Acerba systems
		ace::Update();

		// Draws tilemap
		ace::GraphicsDevice::Draw(tilemap);

		// Refresh screen
		window.Present();
	}

	// Shutdown Acerba
	ace::Quit();

	return 0;
}