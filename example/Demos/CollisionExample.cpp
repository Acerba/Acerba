#include <Ace/Ace.h>

#include <memory> // For custom game object
#include <vector> // For custom game object

// Custom game object
struct MyObject final
{
    // Store a pointer to an object of a pure virtual base class.
    std::unique_ptr<ace::Collidable> collidable;
    // Sprite to draw
    ace::Sprite sprite;

    // Ctor
    MyObject() : collidable(), sprite() { }
};

// Manual vertex placement due to accessing raw GraphicsDevice
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
    // Initialize the engine
    ace::Init();
    
    // Create a window
    ace::Window window("CollisionExample", 800u, 600u);
    
    // Number of collidables
    static const ace::UInt8 collidableCount = 20u;
    
    // Preallocate space for the collidables + player collidable
    ace::Collidable::Reserve(collidableCount + 1u);

    // Create gameobjects
    std::vector<MyObject> objects(collidableCount);
    
    // Size for the objects
    static const ace::Vector2 rectSize(0.05f, 0.05f);
    
    // Set all gameobjects
    for (ace::UInt8 i = 0u; i < collidableCount; ++i)
    {
        // Randomize a position on the screen
        const ace::Vector2 position(ace::math::Rand(-1.f, 1.f), ace::math::Rand(-1.f, 1.f));

        // Create a rectangle (derived from ace::Collidable)
        objects[i].collidable = std::make_unique<ace::Rectangle>(
            rectSize,   // extents
            position    // position
        );

        // Randomize masking for collidables.
        // When collidables have atleast one same bit in their mask, they can collide.
        const bool masked = (ace::math::Rand(0.f, 1.f) < 0.5f);

        // Mask the collidable. Collidables are masked to Mask::A by default.
        if (masked) {
            objects[i].collidable->SetMask(ace::Collidable::Mask::B);
        }

        // Set vertices manually
        SetSprite(objects[i].sprite, rectSize);

        // Move the sprite to the correct position
        objects[i].sprite.Move(ace::Vector3(position.x, position.y, 0.f));

        // Colorize the sprite.
        // Masked collidables are coloured purple and do not register hits.
        // "Unmasked" / default collidables are coloured blue and can be collided with.
        objects[i].sprite.Colorize(
            masked ?
            ace::Color32(0.2f, 0.f, 0.2f, 1.f) :
            ace::Color32(0.f, 0.f, 0.2f, 1.f)
        );
    }

    // Create a player
    MyObject player;

    // Create a rectangle
    player.collidable = std::make_unique<ace::Rectangle>(rectSize, ace::Vector2());

    // Set the vertices manually
    SetSprite(player.sprite, rectSize);

    // Colorize the player
    player.sprite.Colorize(ace::Color32(0.f, 0.2f, 0.f, 1.f));
    
    
    // While the window is open
    while (window)
    {
        // Clear the window
        window.Clear();

        // Update the engines systems
        ace::Update();
        
        // Close the window on key press 1
        if (ace::Keyboard::GetKey("1"))
        {
            window.Close();
        }
        // Move the player left
        if (ace::Keyboard::GetKey("A"))
        {
            player.collidable->GetLocalPosition().x -= 0.01f;
            player.sprite.Move(ace::Vector3(-0.01f, 0.f, 0.f));
        }
        // Move the player right
        if (ace::Keyboard::GetKey("D"))
        {
            player.collidable->GetLocalPosition().x += 0.01f;
            player.sprite.Move(ace::Vector3( 0.01f, 0.f, 0.f));
        }
        // Move the player up
        if (ace::Keyboard::GetKey("W"))
        {
            player.collidable->GetLocalPosition().y += 0.01f;
            player.sprite.Move(ace::Vector3(0.f,  0.01f, 0.f));
        }
        // Move the player down
        if (ace::Keyboard::GetKey("S"))
        {
            player.collidable->GetLocalPosition().y -= 0.01f;
            player.sprite.Move(ace::Vector3(0.f, -0.01f, 0.f));
        }

        // Update the Bounding Volume Hierarchy which contains the actual ace::Collidable implementations
        // This makes sure that there are as few collisions to check as possible when some form of UpdateCollisions is called
        ace::BVH::Update();

        // Update only the collisions regarding the player
        player.collidable->UpdateCollisions();

        // We don't need to know anyone elses collisions in this demo,
        // but there is the following function available in ace::BVH
        // which updates the collisions on every living collidable:
        // ace::BVH::UpdateAllCollisions();


        // If the player has collisions
        if (player.collidable->GetCollisionCount() != 0)
        {
            // Change the player colour
            player.sprite.Colorize(ace::Color32(0.2f, 0.f, 0.f, 1.f));
        }
        else
        {
            // Set the player colour back to original
            player.sprite.Colorize(ace::Color32(0.f, 0.2f, 0.f, 1.f));
        }

        // Print the number of collisions
        // Possibly bad performance on windows
        // ace::Logger::LogInfo("Player is colliding with %i objects.", player.collidable->GetCollisionCount());

        // Draw the game objects
        for (ace::UInt8 i = 0u; i < collidableCount; ++i)
        {
            ace::GraphicsDevice::Draw(objects[i].sprite);
        }
        // Draw the player last so it stays on top on the screen
        ace::GraphicsDevice::Draw(player.sprite);

        // Refresh the screen
        window.Present();

    } // while(window)
    
    // Shutdown the engine
    ace::Quit();

    // Exit the application
    return 0;
}
