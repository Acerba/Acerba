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
		const UInt32 id = UUID<void>::GetID();
		m_particles.emplace_back(position, velocity, id, lifeTime);
		m_sprites.emplace_back(sprite, id);
	}

	Particle::Particle(const SpriteSheet& spriteSheet, const float lifeTime, const Vector3& position, const Vector3& velocity)
	{
		const UInt32 id = UUID<void>::GetID();
		m_particles.emplace_back(position, velocity, id, lifeTime);
		m_sheet = spriteSheet;
		Sprite sprite;
		sprite.SetSprite(m_sheet.GetSprite(0));
		m_sprites.emplace_back(sprite, id);
	}

	Particle::~Particle()
	{

	}

	Particle::ParticleData::ParticleData(
		const Vector3& position,
		const Vector3& velocity,
		const UInt32 id,
		const float lifeTime
	) :
		position(position),
		velocity(velocity),
		ID(id),
		lifeTime(lifeTime),
		originalPosition(position),
		originalLifeTime(lifeTime)
	{

	}

	Particle::SpriteData::SpriteData(const Sprite& sprite, const UInt32 id) :
		sprite(sprite),
		ID(id)
	{

	}

	void Particle::Draw() 
	{
		//	Sort particle sprites, based on isAlive
		std::vector<Sprite> toDraw;
		for (const auto& s : m_sprites)
		{
			const auto itr = std::find_if(
				m_particles.begin(),
				m_particles.end(),
				[&](const ParticleData& p) -> bool
			{
				return s.ID == p.ID;
			});
			if (itr != m_particles.end() && itr->isAlive)
			{
				toDraw.emplace_back(s.sprite);
			}
		}
		//	Draw alive particle sprites 
		GraphicsDevice::Draw(toDraw.data(), toDraw.size(), m_indexBuffer);
	}

	void Particle::Reset()
	{
		const UInt32 size = m_particles.size();
		for (UInt32 i = 0u ; i < size; ++i)
		{
			m_particles[i].position = m_particles[i].originalPosition;
			m_particles[i].isAlive = true;
			m_particles[i].lifeTime = m_particles[i].originalLifeTime;
		}

		m_isAlive = true;
	}
	 
	void Particle::Update()
	{
		//	Get frametime
		const float deltaTime = Time::DeltaTime();
		
		for(UInt32 i = 0; i < m_particles.size(); ++i)
		{ 
			//	Decrease particle lifetime by frametime
			m_particles[i].lifeTime -= deltaTime;

			if (m_particles[i].lifeTime < 0.f)
			{
				m_particles[i].isAlive = false;
			}
			else
			{
				//	Move particle to new location
				m_sprites[i].sprite.Move(math::Scale(m_particles[i].velocity, deltaTime));
			}

			if (std::all_of(m_particles.begin(), m_particles.end(),
				[](const ParticleData& p) -> bool { return !p.isAlive; }
			))
			{
				m_isAlive = false;
			}
		}

		//	Sort particles based on lifetime
		std::sort(m_particles.begin(), m_particles.end());
	}
}


