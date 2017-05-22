#include <Ace/Ace.h>
int main(int, char**) {
    ace::Init();
    ace::Window window("Scene_demo", 1024u, 768u);
    std::array<ace::Entity, 10u> entities;
    ace::Time timer;
    ace::Scene world;
    ace::Camera camera;
    ace::Sprite sprite;
    ace::StandardMaterial mat;
    float total = 0.f;
    const ace::UInt8 EC = 10u;
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