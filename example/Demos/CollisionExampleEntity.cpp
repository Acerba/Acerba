//SceneDrawingMultipleEntites Demo
// Include all necessary modules
#include <Ace/Ace.h>

#include <Ace/Collidable.h>

#include <Ace/Debugger.h>

#include <iostream>

void logC(const ace::Collidable& c, const char* msg = nullptr)
{
    for (const auto& itr : c.GetVertices())
    {
        ace::LogDebug(itr, msg);
    }
}

int main(int, char**) {
    // Initialize Acerba
    ace::Init();
    
    
    #if 0
    
    using namespace ace;
    
    std::cout << std::boolalpha;
    
    Rectangle r1(Vector2(-5.1, 6), Vector2(1, 1));
    Rectangle r2(Vector2(0.999f, 3), Vector2(1, 1));
    
    Vector2 ex[3]{ {-1, -1}, {1, -1}, {1, 1} };
    Triangle t1(Vector2(-4, 5), ex);
    Circle c1(Vector2(-4.6, 5.6), 1);
    
    logC(t1, "t1vert");
    //logC(r1, "r1vert");
    
    std::cout << "töks: " << Collidable::IsColliding(t1, c1) << '\n';
    
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
    entA->transform.GetPositionRef() = ace::Vector3(-0.75f, 0.f, 0.f);
    ace::Rectangle rectA(ace::Vector2(scale, scale), ace::Vector2(-0.75f, 0.f));
    ace::Rectangle rectB(ace::Vector2(scale, scale), ace::Vector2());
    
    // Scale to 20%
    entA->transform.GetScaleRef() = ace::Vector3(scale, scale, 1.f);
    entB->transform.GetScaleRef() = ace::Vector3(scale, scale, 1.f);
    
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
        
        if (ace::Keyboard::GetKey("A"))
        {
            entA->transform.GetPositionRef().x -= dt;
            rectAref.GetLocalPosition().x -= dt;
        }
        if (ace::Keyboard::GetKey("D"))
        {
            entA->transform.GetPositionRef().x += dt;
            rectAref.GetLocalPosition().x += dt;
        }
        if (ace::Keyboard::GetKey("W"))
        {
            entA->transform.GetPositionRef().y += dt;
            rectAref.GetLocalPosition().y += dt;
        }
        if (ace::Keyboard::GetKey("S"))
        {
            entA->transform.GetPositionRef().y -= dt;
            rectAref.GetLocalPosition().y -= dt;
        }
        if (ace::Keyboard::GetKey("1"))
        {
            window.Close();
        }
        
        if (ace::Collidable::IsColliding(rectAref, rectBref))
        {
            spriteBref.Colorize(ace::Color32(0.5f, 0.2f, 0.2f, 1.f));
        }
        else
        {
            (*entB.GetComponent<ace::Sprite>())->Colorize(ace::Color32(0.2f, 0.5f, 0.2f, 1.f));
        }

        // static float angle1 = 0.f;
        // angle1 += dt;
        // cam->transform.rotation = mv::MakeQuaternion(mv::MakeVektor<3u, float>(0, 0, angle1), true);
        
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