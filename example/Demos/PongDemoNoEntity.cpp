//Pong demo

#include <Ace/Ace.h>

#include <iostream>
#include <string>


//	If using android
#if ACE_ANDROID

#include <sstream>

namespace std
{
    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
}

#endif

//	This will flip sprite around
void FlipSprite(ace::Sprite& sprite)
{
    ace::Sprite temp = sprite;
    
    sprite.vertexData[0].uv.x = temp.vertexData[2].uv.x;
    sprite.vertexData[1].uv.x = temp.vertexData[3].uv.x;
    sprite.vertexData[2].uv.x = temp.vertexData[0].uv.x;
    sprite.vertexData[3].uv.x = temp.vertexData[1].uv.x;
    
}

//	This will draw sprites
void drawSprite(ace::Sprite& sprite, ace::StandardMaterial& material, ace::Texture& texture, const ace::Vector2& position, const ace::Vector2& scale)
{
    material->position = position;
    material->scale = scale;
    material->diffuse = texture;
    ace::GraphicsDevice::Draw(sprite);
}

int main(int, char**)
{
    //	Initalize ACE-engine
    ace::Init();
    //	Create window
    ace::Window window("Pong Demo", 1200, 800);
    //	Set window clear color
    ace::Color color_Green = ace::Color(0, 255, 0, 0);
    window.SetClearColor(color_Green);
    // Create camera
    ace::Camera mainCamera;
    
    //	Add and load sounds/music
    ace::AudioClip sound_Ding;
    ace::AudioClip sound_Dong;
    ace::AudioClip sound_GameOver;
    ace::AudioStream m_Music;
    bool useAudio = false;
    
    if (ace::File::Exists("assets/ding.wav"))
    {
        useAudio = true;
        sound_Ding = ace::AudioClip({ "assets/ding.wav" });
    }
    
    if (ace::File::Exists("assets/dong.wav"))
    {
        useAudio = true;
        sound_Dong = ace::AudioClip({ "assets/dong.wav" });
    }
    
    if (ace::File::Exists("assets/gameover.wav"))
    {
        useAudio = true;
        sound_GameOver = ace::AudioClip({ "assets/gameover.wav" });
    }
    
    if (ace::File::Exists("assets/music.wav"))
    {
        useAudio = true;
        m_Music = ace::AudioStream({ "assets/music.wav" }, 1.0f, true, true);
    }
    
    //	Add and load images
    ace::Image img_Ball(0xFFFFFFFFU);
    ace::Image img_Enemy(0xFFFFFFFFU);
    ace::Image img_Player(0xFFFFFFFFU);
    
    if (ace::File::Exists("assets/ball.png"))
    {
        img_Ball = ace::Image(ace::File("assets/ball.png"));
    }
    
    if (ace::File::Exists("assets/enemy.png"))
    {
        img_Enemy = ace::Image(ace::File("assets/enemy.png"));
    }
    
    if (ace::File::Exists("assets/player.png"))
    {
        img_Player = ace::Image(ace::File("assets/player.png"));
    }
    
    
    // Add and load font
    ace::Font font_Arial;
    
    if (ace::File::Exists("assets/arial.ttf"))
    {
        font_Arial.Fill(ace::File("assets/arial.ttf"));
    }
    
    //	Add and load textures
    ace::Texture tex_Ball(img_Ball);
    ace::Texture tex_Enemy(img_Enemy);
    ace::Texture tex_Player(img_Player);
    
    //	Add and load spritesheet
    ace::SpriteSheet ss_Numbers;
    if (ace::File::Exists("assets/testSheet.json"))
    {
        ss_Numbers = ace::SpriteSheet("assets/testSheet.json");
    }
    
    //	Add and load sprites
    ace::Sprite s_Number;
    s_Number.Colorize(ace::Color(255, 255, 255, 255));
    ace::Sprite s_Ball;
    FlipSprite(s_Ball);
    ace::Sprite s_Enemy(90.0f);
    ace::Sprite s_Player(90.0f);
    
    //	Turn enemy sprite towards center
    s_Enemy.vertexData[0].uv.y *= 0.5f;
    s_Enemy.vertexData[1].uv.y *= 0.5f;
    s_Enemy.vertexData[3].uv.y *= 0.5f;
    s_Enemy.vertexData[2].uv.y *= 0.5f;
    
    //	Add and load animation
    ace::Animation ani_Number(ss_Numbers);
    //	Create this to play animation
    int animNumber = 1;
    //	Set animation data to first image and update it
    ani_Number.PlayAnimation("1");
    ani_Number.UpdateAnimation(s_Number);
    //	Set texture
    ace::Texture tex_Number(ss_Numbers.image);
    
    //	Load standart material
    ace::StandardMaterial mat_Standard;
    
    //	Bake fontsheet
    ace::Image img_FontSheet = font_Arial.BakeFontSheet(920, 920, 64, 32, 255);
    //	Create and load textbuffer
    ace::Buffer buf_Text = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
    font_Arial.GetTextBuffer(buf_Text, "Score: ", 0.25f);
    
    //	Create score
    ace::Int32 Score = 0;
    //	Create Scoreposition
    ace::Vector2 vec2_ScorePos = ace::Vector2{ 0.0f,0.9f };
    //	Create scorebuffer
    ace::Buffer buf_Score = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
    //	Create string and change int score to string
    std::string str_Score = std::to_string(Score);
    //	Create scoretext variables
    ace::UInt32 i = 0, ScoreWord;
    
    //	Create FontSheet texture using FontSheet image
    ace::Texture tex_FontSheet(img_FontSheet);
    
    
    // Enable blend and depth for graphicsdevice
    ace::GraphicsDevice::Enable(true, ace::Features::Blend | ace::Features::Depth);
    
    //	Set positions for ball, enemy and player
    ace::Vector2 vec2_BallPos = ace::Vector2{ 0.0f, 0.0f };
    ace::Vector2 vec2_EnemyPos = ace::Vector2{ -0.9f, 0.0f };
    ace::Vector2 vec2_PlayerPos = ace::Vector2{ 0.9f, 0.0f };
    
    //	Create few booleans for ball moving and hitting player
    bool isBallMovingRight = false;
    bool isBallHittingPlayer = false;
    
    //	Add player offset
    static const float PLAYER_OFFSET_Y = 0.32f;
    
    //	Create ball moving abilities
    float xMove = 0.01f;	//X-axis movement speed
    float yMove = 0.01f;	//Y-axis movement speed
    
    float yMax = 0.025f;	//X-axis maximum speed
    float xMax = 0.1f;		//Y-axis maximum speed
    
    //	Play music before main loop
    ace::Audio::PlayAudio(m_Music);
    
    //	Game loop
    while (window)
    {
        //	Update ACE-engine
        ace::Update();
        
        //	Exit command in F1 button
        if (ace::Keyboard::GetKey("F1"))
        {
            //	Add small delay and close window
            ace::Time::Delay(100);
            window.Close();
        }
        
        //	Create scoreword and update current score from int to string and push it to textbuffer
        ScoreWord = std::string("Score: ").size();
        str_Score = std::to_string(Score);
        font_Arial.GetTextBuffer(buf_Score, str_Score.c_str(), 0.25f);
        
        //	Create ball movement when hitting "enemy"
        if (vec2_BallPos.x < -0.8f)
        {
            //	If ball is "hitting" enemy, flip it around
            isBallMovingRight = !isBallMovingRight;
            FlipSprite(s_Ball);
            
            //	Play sound for "hitting" enemy
            if (useAudio)
            {
                ace::Audio::PlayAudio(sound_Ding);
            }
            
            //	If ball is moving under center line of Y-axis, move it down else move it up
            if (yMove <= yMax)
            {
                yMove += 0.001f;
            }
            
            //Flip movement on Y-axis
            yMove = -yMove;
        }
        
        //	Create ball hitting player logic
        if (isBallHittingPlayer)
        {
            //	Flip ball movement and sprite
            FlipSprite(s_Ball);
            isBallMovingRight = !isBallMovingRight;
            isBallHittingPlayer = false;
            
            //	Update animation for "counter"
            if (animNumber == 4)
            {
                ani_Number.PlayAnimation("1");
                ani_Number.UpdateAnimation(s_Number);
                animNumber = 1;
            }
            else if (animNumber == 1)
            {
                ani_Number.PlayAnimation("2");
                ani_Number.UpdateAnimation(s_Number);
                animNumber = 2;
            }
            else if (animNumber == 2)
            {
                ani_Number.PlayAnimation("3");
                ani_Number.UpdateAnimation(s_Number);
                animNumber = 3;
            }
            else if (animNumber == 3)
            {
                ani_Number.PlayAnimation("4");
                ani_Number.UpdateAnimation(s_Number);
                animNumber = 4;
            }
            
            //	Play sound for hitting player
            if (useAudio)
            {
                ace::Audio::PlayAudio(sound_Dong);
            }
            
            //Flip movement on Y-axis
            yMove = -yMove;
        }
        
        //	Make ball flip on Y-axis if hitting top or bottom
        if (vec2_BallPos.y >= 0.95f || vec2_BallPos.y <= -0.95f)
        {
            yMove = -yMove;
        }
        
        //	Make ball movement logic on X-axis
        if (isBallMovingRight)
        {
            vec2_BallPos += ace::Vector2{ xMove, yMove };
        }
        else
        {
            vec2_BallPos += ace::Vector2{ -xMove, -yMove };
        }
        
        //	Create ball "collision" statement
        bool playerCollision = vec2_BallPos.x > vec2_PlayerPos.x - 0.1f && !(vec2_BallPos.y > (vec2_PlayerPos.y + PLAYER_OFFSET_Y) || vec2_BallPos.y < (vec2_PlayerPos.y - PLAYER_OFFSET_Y));
        
        //	Ball and player "collision" 
        if (playerCollision)
        {
            //	Add points to score and make this new score a string
            Score += 10;
            str_Score = std::to_string(Score);
            //	Set playerhitting boolean to true
            isBallHittingPlayer = true;
            
            //	Increase ball movement speed
            if (xMove < xMax)
            {
                xMove += 0.001f;
            }
        }
        
        //	Create losing condition
        if (vec2_BallPos.x >= 0.9f)
        {
            //	Stop ball movement
            xMove = 0.0f;
            yMove = 0.0f;
            
            //	Play game over sound
            if (useAudio)
            {
                ace::Audio::PlayAudio(sound_GameOver);
            }
            
            //	Add delay
            ace::Time::Delay(2250);
            
            //Close window and exit while loop
            window.Close();
            break;
        }
        
        // If current device is android, use this preset
        #if ACE_ANDROID
        
        float speed = ace::Accelerometer::GetY();
        vec2_PlayerPos += ace::Vector2{ 0, speed } *ace::Time::DeltaTime();
        
        if (speed > 0)
        {
            if (!(vec2_PlayerPos.y < 0.75))
            {
                vec2_PlayerPos.y = 0.74;
            }
        }
        else if (speed < 0)
        {
            if (!(vec2_PlayerPos.y > -0.75))
            {
                vec2_PlayerPos.y = -0.74;
            }
        }
        
        // If current device is something else, use this preset
        #else
        
        // Create player movement up
        if (ace::Keyboard::GetKey("W"))
        {
            if (vec2_PlayerPos.y < 0.75f)
            {
                vec2_PlayerPos += ace::Vector2(0.f, 1.5f) * ace::Time::DeltaTime();
            }
            else
            {
                vec2_PlayerPos.y = 0.74f;
            }
        }
        // Create player movement down
        if (ace::Keyboard::GetKey("S"))
        {
            if (vec2_PlayerPos.y > -0.75f)
            {
                vec2_PlayerPos += ace::Vector2(0.f, -1.5f) * ace::Time::DeltaTime();
            }
            else
            {
                vec2_PlayerPos.y = -0.74f;
            }
        }
        
        #endif	
        
        
        //	Clear window 
        window.Clear();
        
        //	Draw number "animation"
        drawSprite(s_Number, mat_Standard, tex_Number, ace::Vector2(0.f,0.f), ace::Vector2(0.5f, 0.5f));
        
        //	Draw enemy, player and ball
        drawSprite(s_Enemy, mat_Standard, tex_Enemy, vec2_EnemyPos, { 0.2f,2.0f });
        drawSprite(s_Player, mat_Standard, tex_Player, vec2_PlayerPos, { 0.2f,0.5f });
        drawSprite(s_Ball, mat_Standard, tex_Ball, vec2_BallPos, { 0.2f,0.2f });
        
        //	Draw score text and score
        mat_Standard->position = vec2_ScorePos;
        mat_Standard->scale = ace::Vector2(0.01f, 0.01f);
        mat_Standard->diffuse = tex_FontSheet;
        ace::GraphicsDevice::SetBuffer(buf_Text);
        ace::Int32 TextBufferSize = buf_Text.size / (ScoreWord)* (i % (ScoreWord)+1);
        ace::GraphicsDevice::Draw(TextBufferSize * 6, 0);
        
        mat_Standard->position = vec2_ScorePos - ace::Vector2(0.f ,0.2f);
        mat_Standard->scale = ace::Vector2(0.01f, 0.01f);
        mat_Standard->diffuse = tex_FontSheet;
        ace::GraphicsDevice::SetBuffer(buf_Score);
        ScoreWord = str_Score.size();
        TextBufferSize = buf_Score.size / (ScoreWord)* (i % (ScoreWord)+1);
        ace::GraphicsDevice::Draw(TextBufferSize * 6, 0);
        
        //	Window updating
        window.Present();
    }
    
    //	Quit ace
    ace::Quit();
    return 0;
}