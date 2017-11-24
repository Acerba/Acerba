#include <Ace/Module.h>
#include <Ace/Ace.h>
#include <Ace/ParticleSystem.h>
#include <iostream>

#include <Ace/GLMaterialImpl.h> // TODO: remove

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
	ace::Sprite sprite3;

	ace::math::Vector3 particlePosition( 0.0f,0.0f,0.0f );
	ace::math::Vector3 particleVelocity( 0.05f,0.0f,0.0f );
	ace::math::Vector3 particleVelocity2(-0.05f, 0.0f, 0.0f);
	ace::math::Vector3 particleVelocity3(0.0f, -0.05f, 0.0f);
	float spudroLifeTime = 5.0f;
	float testLifeTime = 10.0f;
	float ebinLifeTime = 10.0f;

	ace::Particle spudroParticle(sprite1, spudroLifeTime, particlePosition, particleVelocity);
	ace::Particle testParticle(sprite2, testLifeTime, particlePosition, particleVelocity2);

	ace::Particle spudroparticle2(sprite3, ebinLifeTime, particlePosition, particleVelocity3);

	
	
	ace::math::Vector3 particleSystemPosition(0.0f, 0.0f, 0.0f);
	ace::math::Vector3 particleSystemPosition2(0.25f, 0.25f, 1.0f);
	bool partSysActive = true;
	ace::ParticleSystem spudroParticleSystem(spudroParticle, particleSystemPosition, partSysActive);
	ace::ParticleSystem testParticleSystem(testParticle, particleSystemPosition2, partSysActive);

	spudroParticleSystem.AddParticle(spudroparticle2);

	// TODO: set without impl
	static_cast<ace::StandardMaterial&>(spudroParticleSystem.GetMaterial())->diffuse = SpudroTexture;
	static_cast<ace::StandardMaterial&>(testParticleSystem.GetMaterial())->diffuse = TestImageTexture;

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

		if (ace::Keyboard::GetKey("Space"))
		{
			spudroParticle.Reset();
			std::cout << "Ebin";
		}

		Window.Clear({ 0x0008400FFU });
		spudroParticleSystem.Update();
		testParticleSystem.Update();

		theScene.Update();
		theScene.Draw(theCamera);
		Window.Present();
	}

    ace::Quit();
}