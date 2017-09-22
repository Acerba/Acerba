// Include all necessary modules
#include <Ace/Ace.h>

int main(int, char**)
{
    // Initialize Acerba
    ace::Init();

    // Create a window
    ace::Window myWindow("Scene demo", 800u, 600u);


    // Create a material
    ace::StandardMaterial myMaterial;


    // SPRITE

    // Create a sprite
    ace::Sprite mySprite;

    // Create a colour
    ace::Color myColour(0u, 255u, 0u, 0u);

    // Change the colour of the sprite
    mySprite.Colorize(myColour);


    // WORLD

    // Create a scene
    ace::Scene myWorld;

    // Create a camera
    ace::Camera myCamera;

    // Set world root as the parent of myCamera
    myCamera.GetEntity().SetParent(myWorld.GetRoot());


    // ENTITY

    // Create an entity
    ace::Entity myEntity;

    // Add the modified sprite to the component
    myEntity.AddComponent(mySprite);

    // Add the standard material to the entity as a normal material
    myEntity.AddComponent<ace::Material>(myMaterial);

    // Add entity as a child of the camera
    myCamera.GetEntity().AddChild(myEntity);



    // Which way is the entity moving
    bool moveRight = true;

    // While window is active
    while (myWindow) {

        // Update Acerba systems
        ace::Update();

        // Clear the window
        myWindow.Clear();

        // Get reference to the x-axis position
        float& x = myEntity->transform.position.x;

        // Limit movement
        if (x >= 1.f)
            moveRight = false;
        else if (x <= -1.f)
            moveRight = true;

        // Move the entity
        x += (0.01f * (moveRight ? 1.f : -1.f));

        // Update the world
        myWorld.Update();

        // Draw the world and all its children
        myWorld.Draw(myCamera);

        // Refresh the window
        myWindow.Present();
    }

    // Shutdown Acerba
    ace::Quit();

    return 0;
}
