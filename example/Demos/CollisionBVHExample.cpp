#include <Ace/Ace.h>

#include <Ace/Collidable.h>

int main(int, char**)
{
    ace::Init();

    ace::Window window("BVHExample", 800u, 600u);

    ace::Collidable::Reserve(4u);

    ace::Rectangle r1(ace::Vector2(2.f, 1.f), ace::Vector2(0.f, 0.f));
    ace::Rectangle r2(ace::Vector2(2.f, 1.f), ace::Vector2(5.f, 5.f));

    ace::Sprite s1;
    ace::Sprite s2;

    s1.Colorize(ace::Color32(0.2f, 0.f, 0.f, 1.f));
    s2.Colorize(ace::Color32(0.f, 0.2f, 0.f, 1.f));

    // r1.GetShared();
    // ace::BVH::AddCollidable(r1);
    // ace::BVH::AddCollidable(r2);


    while (window)
    {
        window.Clear();
        ace::Update();

        // ace::GraphicsDevice::Draw(s1);
        // ace::GraphicsDevice::Draw(s2);


        if (ace::Keyboard::GetKey("1"))
        {
            window.Close();
        }
        if (ace::Keyboard::GetKey("4"))
        {
            r1.UpdateCollisions();
        }


        window.Present();
    }

    ace::Quit();
    return 0;
}
