#include <Ace/Ace.h>

#include <Ace/Collidable.h>
#include <Ace/CollidableImpl.h>
#include <Ace/BVH.h>

#include <memory>
#include <vector>

struct MyObject final
{
    std::unique_ptr<ace::Collidable> collidable;
    ace::Sprite sprite;

    MyObject() : collidable(), sprite() { }
};

void SetSprite(ace::Sprite& sprite, const ace::Vector2& pos)
{
    sprite.vertexData[0].position.x =  pos.x;
    sprite.vertexData[0].position.y =  pos.y;

    sprite.vertexData[1].position.x = -pos.x;
    sprite.vertexData[1].position.y =  pos.y;

    sprite.vertexData[2].position.x = -pos.x;
    sprite.vertexData[2].position.y = -pos.y;

    sprite.vertexData[3].position.x =  pos.x;
    sprite.vertexData[3].position.y = -pos.y;
}

int main(int, char**)
{
    ace::Init();
    
    ace::Window window("BVHExample", 800u, 600u);
    
    static const ace::UInt8 collidableCount = 20u;
    
    ace::Collidable::Reserve(collidableCount + 1u);
    std::vector<MyObject> objects(collidableCount);
    
    static const ace::Vector2 rectSize(0.05f, 0.05f);
    
    for (ace::UInt8 i = 0u; i < collidableCount; ++i)
    {
        const ace::Vector2 position(ace::math::Rand(-1.f, 1.f), ace::math::Rand(-1.f, 1.f));
        objects[i].collidable = std::make_unique<ace::Rectangle>(
            rectSize,   // extents
            position    // position
        );
        SetSprite(objects[i].sprite, rectSize);
        objects[i].sprite.Move(ace::Vector3(position.x, position.y, 0.f));
        objects[i].sprite.Colorize(ace::Color32(0.f, 0.f, 0.2f, 1.f));
    }

    MyObject player;
    player.collidable = std::make_unique<ace::Rectangle>(rectSize, ace::Vector2());
    SetSprite(player.sprite, rectSize);
    player.sprite.Colorize(ace::Color32(0.f, 0.2f, 0.f, 1.f));
    
    
    while (window)
    {
        window.Clear();
        ace::Update();
        
        if (ace::Keyboard::GetKey("1"))
        {
            window.Close();
        }
        if (ace::Keyboard::GetKey("A"))
        {
            player.collidable->GetLocalPosition().x -= 0.01f;
            player.sprite.Move(ace::Vector3(-0.01f, 0.f, 0.f));
        }
        if (ace::Keyboard::GetKey("D"))
        {
            player.collidable->GetLocalPosition().x += 0.01f;
            player.sprite.Move(ace::Vector3( 0.01f, 0.f, 0.f));
        }
        if (ace::Keyboard::GetKey("W"))
        {
            player.collidable->GetLocalPosition().y += 0.01f;
            player.sprite.Move(ace::Vector3(0.f,  0.01f, 0.f));
        }
        if (ace::Keyboard::GetKey("S"))
        {
            player.collidable->GetLocalPosition().y -= 0.01f;
            player.sprite.Move(ace::Vector3(0.f, -0.01f, 0.f));
        }

        // if (ace::Keyboard::GetKey("4"))
        {
            ace::BVH::Update();
            player.collidable->UpdateCollisions();
        }


        if ((*(*player.collidable.get())).GetCollisionCount() != 0)
        {
            player.sprite.Colorize(ace::Color32(0.2f, 0.f, 0.f, 1.f));
        }
        else
        {
            player.sprite.Colorize(ace::Color32(0.f, 0.2f, 0.f, 1.f));
        }


        for (ace::UInt8 i = 0u; i < collidableCount; ++i)
        {
            ace::GraphicsDevice::Draw(objects[i].sprite);
        }
        ace::GraphicsDevice::Draw(player.sprite);

        window.Present();
    }
    
    ace::Quit();
    return 0;
}
