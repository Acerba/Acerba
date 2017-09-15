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

    // Create a camera
    ace::Camera camera;

    // Create a sprite
    ace::Sprite sprite;

    // Create a material
    ace::StandardMaterial mat;

    
    ace::Entity* target = &world.GetRoot();
    target->AddComponent<ace::Sprite>(sprite);
    sprite.Colorize(ace::Color32(0.5f, 0.5f, 0.5f, 1.f));
    camera.SetParent(*target);
    target = &camera.GetCamera();
    target->AddComponent<ace::Sprite>(sprite);
    target->AddComponent<ace::Material>(mat);
    for (ace::UInt8 i = 0u; i < EC; ++i) {
        entities[i] = ace::Entity();
        entities[i]->transform.position = ace::Vector3(-0.9f + 0.2f*i, 0.f, 0.f);
        entities[i]->transform.scale = ace::Vector3(0.2f, 0.2f, 1.f);
        entities[i].AddComponent<ace::Sprite>(sprite);
        entities[i].AddComponent<ace::Material>(mat);
        target->AddChild(entities[i]);
    }

    float total = 0.f;

    while (window) {
        ace::Update();
        window.Clear(ace::Color(127u, 127u, 127u, 0u));
        total += timer.DeltaTime();
        for (ace::UInt8 i = 0u; i < EC; ++i) {
            entities[i]->transform.position.y = ace::math::Sin(total*(i + 1u)*0.05f);
            entities[i].GetComponent<ace::Sprite>()->operator ace::Sprite*()->Colorize(ace::Color32((i + 1u)*total*0.01f, (i + 1u)*total*0.01f, (i + 1u)*total*0.01f, 1.f));
        }
        world.Update();
        ace::SpriteManager::Draw(world, camera, &mat);
        window.Present();
    }
    ace::Quit();
    return 0;
}