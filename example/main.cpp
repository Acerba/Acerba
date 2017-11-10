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

	img1 = ace::Image(ace::File("spudro.png"));
	ace::Texture SpudroTexture(img1);

	ace::Sprite sprite1;

	ace::math::Vector3 particlePosition( 0.f,0.f,0.f );
	ace::math::Vector3 particleVelocity( 0.5f,0.f,0.f );
	float spudroLifeTime = 10.f;

	ace::Particle spudroParticle(sprite1, spudroLifeTime, particlePosition, particleVelocity);
	ace::math::Vector3 particleSystemPosition(0.5f, 0.25f, 0.f);
	bool partSysActive = true;
	ace::ParticleSystem spudroParticleSystem(spudroParticle, particleSystemPosition, partSysActive);

	spudroParticleSystem.m_standardMaterial->diffuse = SpudroTexture;

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
		spudroParticleSystem.Draw();

		//printf(spudroParticleSystem.m_particle.particles.data()->position.x);
		std::cout << spudroParticleSystem.m_particle.particles.data()->position.x << std::endl;

		theScene.Update();
		theScene.Draw(theCamera);
		Window.Present();
	}

    ace::Quit();
}