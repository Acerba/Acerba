//Animation Demo
#include <Ace/Ace.h>

int main(int, char**) 
{
    // Initialize Acerba
    ace::Init();

    // Create window
    ace::Window window("Animation Demo", 1024, 768);

    // Load Spritesheet
    ace::SpriteSheet sheet("assets/Animation.json");
    
    // Create Animations
    ace::Animation animation(sheet);
    
    ace::StandardMaterial material;
    material->diffuse = animation.GetTexture();
    ace::GraphicsDevice::SetMaterial(material);

    // Edit Animation
    animation.GetAnimation(0)->loop = true;

    ace::Entity entity;
    entity.AddComponent(ace::Sprite());
    
    // Automatic Update
    ace::AnimationComponent* anim = *entity.AddComponent(ace::AnimationComponent(entity, animation));
    anim->Play(0);
    //anim->Play("Run");
    
    ace::Camera camera;
    
    ace::UInt32 frame = 0;
    ace::Sprite sprite;

    sprite.Move(ace::Vector3(0.1f, 0.0f, 0.0f));

    while (window)
    {
        window.Clear();

        ace::Update();
       
        ace::GraphicsDevice::Draw(entity);

        // Manual Animation
        animation.UpdateAnimation(sprite, 0, frame++);
        // or 
        //animation.UpdateAnimation(sprite, "Run", frame++);
        ace::GraphicsDevice::Draw(sprite);


        window.Present();
    }

    // Shutdown Acerba
    ace::Quit();

    return 0;
}