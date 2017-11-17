#include <Ace/Module.h>
#include <Ace/Ace.h>
#include <Ace/ParticleSystem.h>
#include <iostream>

int main(int, char**)
{
    ace::Init();
	ace::Window Window("ParticleDemo", 1200, 800);
	ace::Scene theScene;
	ace::Camera theCamera;

	theScene.GetRoot().AddChild(theCamera.GetEntity());


	ace::Image img1(0xFFFFFFFFU);
	ace::Image img2(0xFFFFFFFFU);
	img1 = ace::Image(ace::File("spudro.png"));
	img2 = ace::Image(ace::File("TestImageFile.png"));
	ace::Texture SpudroTexture(img1);
	ace::Texture TestImageTexture(img2);

	ace::Sprite sprite1;
	ace::Sprite sprite2;

	//ace::SpriteSheet spriteSheet1("sheet/Kebab.png");

	ace::math::Vector3 particlePosition( 0.0f,0.0f,0.0f );
	ace::math::Vector3 particleVelocity( 0.05f,0.0f,0.0f );
	ace::math::Vector3 particleVelocity2(-0.05f, 0.0f, 0.0f);
	float spudroLifeTime = 5.0f;
	float testLifeTime = 10.0f;

	ace::Particle spudroParticle(sprite1, spudroLifeTime, particlePosition, particleVelocity);
	ace::Particle testParticle(sprite2, testLifeTime, particlePosition, particleVelocity2);
	
	
	ace::math::Vector3 particleSystemPosition(0.0f, 0.0f, 0.0f);
	ace::math::Vector3 particleSystemPosition2(0.25f, 0.25f, 1.0f);
	bool partSysActive = true;
	ace::ParticleSystem spudroParticleSystem(spudroParticle, particleSystemPosition, partSysActive);
	ace::ParticleSystem testParticleSystem(testParticle, particleSystemPosition2, partSysActive);

	spudroParticleSystem.m_standardMaterial->diffuse = SpudroTexture;
	testParticleSystem.m_standardMaterial->diffuse = TestImageTexture;


	//ace::math::Vector3 Upperposition( 0.0f,0.75f );
	//ace::math::Vector3 Lowerposition( 0.0f, -0.75f );

	while (Window)
	{
		ace::Update();
		theCamera.Apply();
		theCamera.Update();

		if (ace::Keyboard::GetKey("Escape"))
		{
			ace::Time::Delay(100);
			Window.Close();
		}

		Window.Clear({ 0x0008400FFU });
		spudroParticleSystem.Update();
		testParticleSystem.Update();

		std::cout << spudroParticleSystem.m_particle.particles.data()->position.x << std::endl;

		theScene.Update();
		theScene.Draw(theCamera);
		Window.Present();
	}

    ace::Quit();
}