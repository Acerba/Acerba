//SceneDrawingMultipleEntites Demo
// Include all necessary modules
#include <Ace/Ace.h>

#include <Ace/Collidable2D.h>

#include <Ace/Debugger.h>

#include <iostream>

void logC(const ace::Collidable2D& c, const char* msg = nullptr)
{
    for (const auto& itr : c.GetVertices())
    {
        ace::LogDebug(itr, msg);
    }
}

int main(int, char**) {
    // Initialize Acerba
    ace::Init();


    #if 1

    using namespace ace;

    std::cout << std::boolalpha;

    Rectangle r1(Vector2(-5.1, 6), Vector2(1, 1));
    Rectangle r2(Vector2(0.999f, 3), Vector2(1, 1));

    Vector2 ex[3]{ {-1, -1}, {1, -1}, {1, 1} };
    Triangle t1(Vector2(-4, 5), ex);
    Circle c1(Vector2(-4.6, 5.6), 1);

    logC(t1, "t1vert");
    //logC(r1, "r1vert");

    std::cout << "töks: " << Collidable2D::IsColliding(t1, c1) << '\n';

    #else

    // Create window
    ace::Window window("Scene_demo", 1024u, 768u);

    ace::Entity entA;
    ace::Entity entB;

    // Create a timer
    ace::Time timer;

    // Create a scene
    ace::Scene world;

    // Create a sprite
    ace::Sprite spriteA;
    ace::Sprite spriteB;

    // Create a material
    ace::StandardMaterial mat;

    // Change sprite colour
    spriteA.Colorize(ace::Color32(0.f, 0.f, 0.f, 1.f));
    spriteB.Colorize(ace::Color32(1.f, 1.f, 1.f, 1.f));

    // Create a camera
    ace::Camera camera;

    // Set world root as parent of camera
    //camera.GetEntity().SetParent(world.GetRoot());

    // Take camera reference so we don't have to get it every time
    ace::Entity& cam = camera.GetEntity();

    const float scale = 0.2f;

    // Change position
    entA->transform.position = ace::Vector3(-0.75f, 0.f, 0.f);
    ace::Rectangle rectA(ace::Vector2(-0.75f, 0.f), ace::Vector2(scale, scale));
    ace::Rectangle rectB(ace::Vector2(), ace::Vector2(scale, scale));

    // Scale to 20%
    entA->transform.scale = ace::Vector3(scale, scale, 1.f);
    entB->transform.scale = ace::Vector3(scale, scale, 1.f);

    // Add Sprite to entity
    entA.ReserveComponents<ace::Sprite>(2u);

    ace::Sprite& spriteAref = entA.AddComponent(spriteA)->GetRef();
    ace::Sprite& spriteBref = entB.AddComponent(spriteB)->GetRef();


    entA.ReserveComponents<ace::Rectangle>(2u);

    ace::Rectangle& rectAref = entA.AddComponent(rectA)->GetRef();
    ace::Rectangle& rectBref = entB.AddComponent(rectB)->GetRef();

    // Add material to entity
    entA.AddComponent<ace::Material>(mat);
    entB.AddComponent<ace::Material>(mat);

    entA.SetParent(world.GetRoot());
    entB.SetParent(world.GetRoot());

    #if 0
        camera.GetEntity().SetParent(world.GetRoot());
    #else
        camera.GetEntity().SetParent(entB);
    #endif

    // Total time elapsed, used in color calculation
    float dt = 0.f;

    // While window is open
    while (window) {

        // Clear window
        window.Clear();

        // Update Acerba systems
        ace::Update();

        // Get time between updates
        dt = timer.DeltaTime();

        if (ace::Keyboard::KeyPressed(ace::KeyCode::A))
        {
            entA->transform.position.x -= dt;
            rectAref.GetPosition().x -= dt;
        }
        if (ace::Keyboard::KeyPressed(ace::KeyCode::D))
        {
            entA->transform.position.x += dt;
            rectAref.GetPosition().x += dt;
        }
        if (ace::Keyboard::KeyPressed(ace::KeyCode::W))
        {
            entA->transform.position.y += dt;
            rectAref.GetPosition().y += dt;
        }
        if (ace::Keyboard::KeyPressed(ace::KeyCode::S))
        {
            entA->transform.position.y -= dt;
            rectAref.GetPosition().y -= dt;
        }
        if (ace::Keyboard::KeyPressed(ace::KeyCode::One))
        {
            window.Close();
        }

        if (ace::Collidable2D::IsColliding(rectAref, rectBref))
        {
            static ace::Color col(1.f, 1.f, 1.f, 1.f);

            col = ace::Color(0.5f, 0.2f, 0.2f, 1.f);
            spriteBref.Colorize(col);
        }
//        else
//        {
//            (*entB.GetComponent<ace::Sprite>())->Colorize(ace::Color(0.2f, 0.5f, 0.2f, 1.f));
//        }

        // Update scene
        world.Update();

        // Draw scene
        world.Draw(camera);

        // Refresh screen
        window.Present();
    }

    #endif

    // Shutdown Acerba
    ace::Quit();

    return 0;
}