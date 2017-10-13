#include <Ace\Ace.h>

#include <string>
#include <sstream>

#if ACE_ANDROID

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


void update()
{


}

void Flip(ace::Sprite& sprite)
{
	ace::Sprite temp = sprite;

	sprite.vertexData[0].uv.x = temp.vertexData[2].uv.x;
	sprite.vertexData[1].uv.x = temp.vertexData[3].uv.x;
	sprite.vertexData[2].uv.x = temp.vertexData[0].uv.x;
	sprite.vertexData[3].uv.x = temp.vertexData[1].uv.x;

}

void draw(ace::Material& material, ace::Sprite& sprite, ace::Texture& texture, const ace::Vector2& position, const ace::Vector2& scale)
{
	material.Uniform("position", position);
	material.Uniform("scale", scale);

	ace::GraphicsDevice::SetTexture(texture, "Texture", 0);
	ace::GraphicsDevice::Draw(sprite);
}

int main(int, char**)
{
	ace::Init();

	ace::Window window("PongDemo", 1200, 800);

	ace::AudioClip HitSound1;
	ace::AudioClip HitSound2;
	ace::AudioClip s_GameOver;
	ace::AudioStream Music1;
	bool useAudio = false;

	if (ace::File::Exists("assets/ding.wav"))
	{
		useAudio = true;
		HitSound1 = ace::AudioClip({ "assets/ding.wav" });
	}

	if (ace::File::Exists("assets/dong.wav"))
	{
		useAudio = true;
		HitSound2 = ace::AudioClip({ "assets/dong.wav" });
	}

	if (ace::File::Exists("assets/music.wav"))
	{
		useAudio = true;
		Music1 = ace::AudioStream({ "assets/music.wav" }, 1.0f, true, true);
	}

	if (ace::File::Exists("assets/gameover.wav"))
	{
		//Rrorr
		useAudio = true;
		s_GameOver = ace::AudioClip({ "assets/gameover.wav" });
	}

	// Loading images
	ace::Image Ball(0xFFFFFFFFU);
	ace::Image Enemy(0xFFFFFFFFU);
	ace::Image Player(0xFFFFFFFFU);

	if (ace::File::Exists("assets/ball.png"))
	{
		Ball = ace::Image(ace::File("assets/ball.png"));
	}

	if (ace::File::Exists("assets/enemy.png"))
	{
		Enemy = ace::Image(ace::File("assets/enemy.png"));
	}

	if (ace::File::Exists("assets/player.png"))
	{
		Player = ace::Image(ace::File("assets/player.png"));
	}

	//Loading textures
	ace::Texture BallTexture(Ball);
	ace::Texture EnemyTexture(Enemy);
	ace::Texture PlayerTexture(Player);

	//Loading spritesheets
	ace::SpriteSheet Numbers;
	if (ace::File::Exists("assets/testSheet.json"))
	{
		Numbers = ace::SpriteSheet("assets/testSheet.json");
	}

	//Create camera
	ace::Camera camera;

	//Load Sprites
	ace::Sprite NumberSprite;
	NumberSprite.Colorize(ace::Color(255, 255, 255, 255));

	//Loading animations
	ace::Animation NumberAnim(Numbers);


	int animNumber = 1;

	NumberAnim.PlayAnimation("1");
	NumberAnim.UpdateAnimation(NumberSprite);

	ace::Texture n(Numbers.image);

	//Create scene object
	ace::Scene world;
	ace::Entity& wRoot = world.GetRoot();

	ace::Entity test;

	ace::StandardMaterial standardMat;
	standardMat.flags.cullingMode = ace::CullingMode::Both;
	test.AddComponent<ace::Sprite>(NumberSprite);
	test.AddComponent<ace::Material>(standardMat);

	//test.AddComponent<ace::Animation>(NumberAnim);

	wRoot.AddChild(test);



	// Vertex and fragment shaders
	ace::Shader fragment, vertex;
	vertex.Load({ "assets/vertex.vert" }, ace::ShaderType::Vertex);
	fragment.Load({ "assets/fragment.frag" }, ace::ShaderType::Fragment);

	// Material loading
	//ace::Material material(vertex, fragment);
	ace::Material material(vertex, fragment);
	material.flags.cullingMode = ace::CullingMode::Both;

	ace::Sprite BallSprite;
	Flip(BallSprite);
	ace::Sprite EnemySprite(90.0f);
	ace::Sprite PlayerSprite(90.0f);

	EnemySprite.vertexData[0].uv.y /= 2.0f;
	EnemySprite.vertexData[1].uv.y /= 2.0f;
	EnemySprite.vertexData[2].uv.y /= 2.0f;
	EnemySprite.vertexData[3].uv.y /= 2.0f;

	// Loading and making score text
	ace::Int32 Score = 100;
	ace::Font arial(ace::File("assets/arial.ttf"));
	ace::Image FontSheet = arial.BakeFontSheet(920, 920, 64, 32, 255);

	ace::Buffer TextBuffer = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
	arial.GetTextBuffer(TextBuffer, "Score: ", 0.25f);

	ace::Buffer ScoreBuffer = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
	std::string scoreString = std::to_string(Score);

	// Create fontSheet texture using FontSheet image
	ace::Texture fontSheet(FontSheet);
	ace::UInt32 i = 0, l;

	ace::Vector2 ScorePosition = ace::Vector2{ 0.0f, 0.9f };

	// Enabling blend and depth
	ace::GraphicsDevice::Enable(true, ace::Features::Blend | ace::Features::Depth);

	// Positions for enemy, player and ball
	ace::Vector2 BallPosition = ace::Vector2{ 0.0f, 0.0f };
	ace::Vector2 EnemyPosition = ace::Vector2{ -0.9f, 0 };
	ace::Vector2 PlayerPosition = ace::Vector2{ 0.9f, 0 };;

	// Booleans for the ball
	bool BallRight = false;
	bool BallHitPlayer = false;

	static const float PLAYER_OFFSET_Y = 0.32f;

	// Ball mocing speeds
	float xMove = 0.01f;
	float yMove = 0.001f;

	// Ballmaximum moving speeds
	float xMax = 0.02f;
	float yMax = 0.35f;

	ace::Audio::PlayAudio(Music1);

	//GameLoop
	while (window)
	{
		update();
		ace::Update();

		// Score

		l = strlen("Score: ");
		scoreString = std::to_string(Score);
		arial.GetTextBuffer(ScoreBuffer, scoreString.c_str(), 0.25f);


		//Exit
		if (ace::Keyboard::KeyPressed(ace::KeyCode::F1))
		{
			ace::Time::Delay(100);
			window.Close();
		}


		// Ball movement
		// Ball heading on X-axis
		if (BallPosition.x < -0.8f)
		{
			BallRight = !BallRight;
			Flip(BallSprite);

			if (useAudio)
			{
				ace::Audio::PlayAudio(HitSound1);
			}

			if (yMove <= 0.0f)
			{
				yMove = yMove - 0.001f;
			}
			else
			{
				yMove = yMove + 0.001f;
			}
		}

		// Ball hitting player
		if (BallHitPlayer)
		{
			//Flip(BallSprite);
			BallRight = !BallRight;
			BallHitPlayer = false;

			ace::Sprite& num = *(test.GetComponent<ace::Sprite>()->operator->());

			if (animNumber == 4)
			{
				NumberAnim.PlayAnimation("1");
				NumberAnim.UpdateAnimation(num);
				animNumber = 1;
				printf("\nYksi");
			}
			else if (animNumber == 1)
			{
				NumberAnim.PlayAnimation("2");
				NumberAnim.UpdateAnimation(num);
				animNumber = 2;
				printf("\nKaksi");

				NumberSprite.Move(ace::Vector3(30.0f, -30.0f, 0.0f));
			}
			else if (animNumber == 2)
			{
				NumberAnim.PlayAnimation("3");
				NumberAnim.UpdateAnimation(num);
				animNumber = 3;
				printf("\nKolme");
			}
			else if (animNumber == 3)
			{
				NumberAnim.PlayAnimation("4");
				NumberAnim.UpdateAnimation(num);
				printf("\nNelja");
				animNumber = 4;
			}

			if (useAudio)
			{
				ace::Audio::PlayAudio(HitSound2);
			}
		}

		// Ball heading on Y-axis
		if (BallPosition.y >= 0.95f || BallPosition.y <= -0.95f)
		{
			if (yMove <= yMax);
			{
				yMove = -yMove;
			}
		}

		// Ball movement
		if (BallRight)
		{
			BallPosition += ace::Vector2{ xMove, yMove };
		}
		else
		{
			BallPosition += ace::Vector2{ -xMove, yMove };
		}

		// Ball collision statement
		bool playerCollision = BallPosition.x > PlayerPosition.x - 0.1f && !(BallPosition.y > (PlayerPosition.y + PLAYER_OFFSET_Y) || BallPosition.y < (PlayerPosition.y - PLAYER_OFFSET_Y));

		// Ball and player collision
		if (playerCollision)
		{
			Score += 10;
			scoreString = std::to_string(Score);
			BallHitPlayer = true;

			if (xMove < 0.02f)
			{
				xMove = xMove + 0.0001f;
			}
		}

		// Losing game
		if (BallPosition.x > 0.95f)
		{
			xMove = 0.0f;
			yMove = 0.0f;

			if (useAudio)
			{
				ace::Audio::PlayAudio(s_GameOver);
			}

			ace::Time::Delay(1000);

			window.Close();
			break;
		}

#if ACE_WIN

		// Player movement up
		if (ace::Keyboard::KeyPressed(ace::KeyCode::W))
		{
			if (PlayerPosition.y < 0.75f)
			{
				PlayerPosition += ace::Vector2{ 0, 1.0f } *ace::Time::DeltaTime();
			}
			else
			{
				PlayerPosition.y = 0.74f;
			}
		}
		// Player movement down
		if (ace::Keyboard::KeyPressed(ace::KeyCode::S))
		{
			if (PlayerPosition.y > -0.75f)
			{
				PlayerPosition += ace::Vector2{ 0, -1.0f } *ace::Time::DeltaTime();
			}
			else
			{
				PlayerPosition.y = -0.74f;
			}
		}

#elif ACE_ANDROID

		float speed = ace::Accelerometer::GetY();
		PlayerPosition += ace::Vector2{ 0, speed } *ace::Time::DeltaTime();

		if (speed > 0)
		{
			if (!(PlayerPosition.y < 0.75))
			{
				PlayerPosition.y = 0.74;
			}
		}
		else if (speed < 0)
		{
			if (!(PlayerPosition.y > -0.75))
			{
				PlayerPosition.y = -0.74;
			}
		}

#endif	

		window.Clear();



		// Draw AI
		draw(material, EnemySprite, EnemyTexture, EnemyPosition, { 0.2f, 2.0f });

		// Draw Player
		draw(material, PlayerSprite, PlayerTexture, PlayerPosition, { 0.2f, 0.5f });

		// Draw ball
		draw(material, BallSprite, BallTexture, BallPosition, { 0.2f, 0.2f });


		// Draw score text
		material.Uniform("position", ScorePosition);
		material.Uniform("scale", ace::Vector2{ 0.01f, 0.01f });

		ace::GraphicsDevice::SetTexture(fontSheet, "Texture", 0);
		ace::GraphicsDevice::SetBuffer(TextBuffer);

		ace::Int32 TextBufferSize = TextBuffer.size / (l)* (i % (l)+1);
		ace::GraphicsDevice::Draw(material, TextBufferSize * 6, 0);

		// Draw score
		material.Uniform("position", ScorePosition - ace::Vector2{ 0, 0.2f });
		material.Uniform("scale", ace::Vector2{ 0.01f, 0.01f });

		ace::GraphicsDevice::SetTexture(fontSheet, "Texture", 0);
		ace::GraphicsDevice::SetBuffer(ScoreBuffer);

		l = scoreString.size();
		TextBufferSize = ScoreBuffer.size / (l)* (i % (l)+1);

		ace::GraphicsDevice::Draw(material, TextBufferSize * 6, 0);

		world.Update();
		//ace::SpriteManager::Draw(world, camera, &standardMat);
		standardMat.diffuse = n;
		ace::GraphicsDevice::Draw(*(test.GetComponent<ace::Sprite>()->operator->()));

		window.Present();
	}

	ace::Quit();

	return 0;
}
