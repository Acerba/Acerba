// Include all necessary modules
#include <Ace/Ace.h>

int main(int, char**) {
    // Initialize Acerba
    ace::Init();

    // Create window
    ace::Window window("Scene_demo", 1024u, 768u);

    // Number of entities in scene
    const ace::UInt8 EC = 10u;

    // Container for entities
    std::array<ace::Entity, EC> entities;

    // Create a timer
    ace::Time timer;

    // Create a scene
    ace::Scene world;

    // Create a sprite
    ace::Sprite sprite;

    // Create a material
    ace::StandardMaterial mat;
    
    // Change sprite colour
    sprite.Colorize(ace::Color32(0.5f, 0.5f, 0.5f, 1.f));

    // Create a camera
    ace::Camera camera;

    // Set world root as parent of camera
    camera.SetParent(world.GetRoot());

    // Take camera reference so we don't have to get it every time
    ace::Entity& cam = camera.GetCamera();

    // Create entities
    for (ace::UInt8 i = 0u; i < EC; ++i) {
        // Create an entity
        entities[i] = ace::Entity();
        // Change position
        entities[i]->transform.position = ace::Vector3(-0.9f + 0.2f*i, 0.f, 0.f);
        // Scale to 20%
        entities[i]->transform.scale = ace::Vector3(0.2f, 0.2f, 1.f);
        // Add Sprite to entity
        entities[i].AddComponent(sprite);
        // Add material to entity
        entities[i].AddComponent<ace::Material>(mat);
        // Add entity as a child of camera
        cam.AddChild(entities[i]);
    }

    float total = 0.f;

    // While window is open
    while (window) {
        // Update Acerba systems
        ace::Update();
        // Clear window
        window.Clear();
        total += timer.DeltaTime();
        for (ace::UInt8 i = 0u; i < EC; ++i) {
            // Change position for each component
            entities[i]->transform.position.y = ace::math::Sin(total*(i + 1u)*0.05f);
            // Change colour for each component
            entities[i].GetComponent<ace::Sprite>()->operator ace::Sprite*()->Colorize(
                ace::Color32((i + 1u)*total*0.01f, (i + 1u)*total*0.01f, (i + 1u)*total*0.01f, 1.f)
            );
        }
        // Update scene
        world.Update();
        // Draw scene
        ace::SpriteManager::Draw(world, camera, &mat);

        // Refresh screen
        window.Present();
    }

    // Shutdown Acerba
    ace::Quit();

    return 0;
}