#include <Ace/Particle.h>
#include <Ace/Time.h>
#include <Ace/UUID.h>
#include <algorithm>	//std::sort



namespace ace
{
	

	//Particle::Particle(const SpriteSheet& sheet, float lifeTime, Vector3 position)
	//{
	//	
	//}

	Particle::Particle(const Sprite& sprite, const float lifeTime, const Vector3& position, const Vector3& velocity)
	{
		particles.emplace_back(position, velocity, lifeTime);
		sprites.emplace_back(sprite);
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
		ID(UUID<void>::GetID())
	{

	}

	void Particle::Draw()
	{
		// Update
		// 1. Update lifetime, position, scale, etc..
		Update();
		
		// Sort particles using lifetime
		// Particle creation
		// 1. if particle is dead -> reuse
		// 2. else create new particle (particles, sprites)

		std::sort(particles.begin(), particles.end());

		// Draw
		
		// Draw
		m_standardMaterial->position = particles.data()->position;
		GraphicsDevice::Draw(sprites.data(), sprites.size(), indexBuffer);

	}

	void Particle::Update()
	{
		const float deltaTime = Time::DeltaTime();
		particles.data()->lifeTime -= deltaTime;
		particles.data()->position += math::Scale(particles.data()->velocity, deltaTime);
	}
}


