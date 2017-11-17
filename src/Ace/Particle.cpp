#include <Ace/Particle.h>
#include <Ace/Time.h>
#include <Ace/UUID.h>
#include <algorithm>	//std::sort
#include <Ace/GraphicsDevice.h>



namespace ace
{
	

	//Particle::Particle(const SpriteSheet& sheet, float lifeTime, Vector3 position)
	//{
	//	
	//}

	Particle::Particle()
	{

	}

	Particle::Particle(const Sprite& sprite, const float lifeTime, const Vector3& position, const Vector3& velocity)
	{
		particles.emplace_back(position, velocity, lifeTime);
		sprites.emplace_back(sprite);
	}

	Particle::Particle(const SpriteSheet& spriteSheet, const float lifeTime, const Vector3& position, const Vector3& velocity)
	{
		particles.emplace_back(position, velocity, lifeTime);
		m_sheet = spriteSheet;
		Sprite sprite;
		sprite.SetSprite(m_sheet.GetSprite(0));
		//sprites.emplace_back(spriteSheet);
	}

	Particle::~Particle()
	{

	}

	Particle::ParticleData::ParticleData(
		const Vector3& position,
		const Vector3& velocity,
		const float lifeTime
	) :
		position(position),
		velocity(velocity),
		ID(0u),
		lifeTime(lifeTime)
	{

	}

	Particle::SpriteData::SpriteData() :
		sprite(),
		ID(UUID<void>::GetID())
	{

	}

	void Particle::Draw()
	{
		//	Update particles
		Update();
		//	Sort particles based on lifetime
		std::sort(particles.begin(), particles.end());	
		//	Draw particle sprites
		for (UInt32 i = 0; i < sprites.size(); i++)
		{
			GraphicsDevice::Draw(sprites.data(), sprites.size(), indexBuffer);
		}

	}

	void Particle::Update()
	{
		//	Get frametime
		const float deltaTime = Time::DeltaTime();
		
		for(UInt32 i = 0; i < particles.size(); ++i)
		{ 
			//	Decrease particle lifetime by frametime
			particles[i].lifeTime -= deltaTime;

			//	Set particles new position based on particles velocity and frametime
			particles[i].position += math::Scale(particles[i].velocity, deltaTime);

			//	Set particle's sprite scale and position 
			sprites[i].SetScale(ace::Vector2(1.0f, 1.0f));
			sprites[i].Move(particles[i].position);
		}
	}
}


