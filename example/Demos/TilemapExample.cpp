// Tilemap Demo
// Include all necessary modules
// REQUIRES: map.tmx file!

#include <Ace/Ace.h>

void TileCallback(ace::Sprite& sprite, ace::UInt32 ID, ace::UInt32 layer, void* data)
{
    ace::Vector3 center = sprite.GetCenter();
    printf("%i %i: (%.1f %.1f %.1f)\n", layer, ID, center.x, center.y, center.z);

    // tileCount
    ++(*static_cast<ace::UInt32*>(data));

}

int main(int, char**)
{
    // Initialize Acerba
    ace::Init();

    // Create window
    ace::Window window("Tilemap_demo", 1024u, 768u);

    ace::UInt32 tileCount = 0;

    // REQUIRES: map.tmx file!
    // Loads tilemap
    ace::Tilemap tilemap("Assets/map.tmx", 1.0f, ace::Vector3(0.5f, 0.5f, 0.0f));

    // Creates tilemap
    tilemap.CreateTiles();

    // Loads tilemap with callback
    //tilemap.CreateTiles(ace::Vector2(0.0f, 0.0f), 0.0f, TileCallback, &tileCount);

    // Create collisions for tilemap, using ObjectLayer.
    // Supports Triangles and Rectangles.
    //tilemap.CreateCollisions(0);

    // Create another tile map 
    // Normalized offset
    ace::Vector2 offset(1.0f, 0.0f); 
    // Creates same tilemap to other position, (offset, depth) 
    tilemap.CreateTiles(offset, -1.0f);
    tilemap.CreateCollisions(0, offset);
    
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