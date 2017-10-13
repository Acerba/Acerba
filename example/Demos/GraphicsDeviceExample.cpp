#include <Ace/Ace.h>
#include <Ace/StandardMaterial.h>

// This demo uses direct calls to GraphicsDevice to draw.

int main(int, char**)
{
    ace::Init();

    ace::Window window("GraphicsDemo", 800, 600);

    // Create empty texture
    ace::Texture exampleTexture;

    // Try to find the file
    if (ace::File::Exists("../../../example/Demos/GraphicsDeviceExampleFiles/TestImageFile.png"))
    {
        // Create an image and bind it to the texture
        exampleTexture = ace::Image(ace::File("../../../example/Demos/GraphicsDeviceExampleFiles/TestImageFile.png"));
    }
    else
    {
        exampleTexture = ace::Image::MissingFile();
    }

    ace::GraphicsDevice::Enable(true, ace::Features::Blend);

    // Material loading
    ace::StandardMaterial material;
    material.flags.cullingMode = ace::CullingMode::Both;

    ace::Camera camera;

    // Creating a sprite
    ace::Sprite exampleSprite;
    exampleSprite.Scale(exampleTexture, 1.0f);

    while (window)
    {
        window.Clear();
        ace::Update();

        camera.Update(window); // Updates aspect ratio.
        camera.Apply(material); // Updates material projection and view matrices.

        // Drawing sprite
        //First is position for material, second is scaling for material
        //material.position = ace::Vector2(0, 0);
        // material.scale = ace::Vector2(2.0f, 1.0f);

        // Set texture to GraphicsDevice
        material.diffuse = exampleTexture;

        ace::GraphicsDevice::SetMaterial(material);

        // Draw the graphisDevice, uses testure from the bound texture
        ace::GraphicsDevice::Draw(exampleSprite);

        // Refrresh the screen
        window.Present();
    }

    // Shutdown Acerba
    ace::Quit();

    return 0;
}
