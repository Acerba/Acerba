#include <Ace/Particle.h>
#include <Ace/Time.h>

#if 0

namespace ace
{
	

	//Particle::Particle(const SpriteSheet& sheet, float lifeTime, Vector3 position)
	//{
	//	
	//}

	Particle::Particle(const Sprite& sprite, float lifeTime, Vector3 position)
	{
		m_sprite = sprite;
		m_lifeTime = lifeTime;
		m_sprite.SetPosition = position;
	}

	Particle::~Particle()
	{

	}

	void Particle::Draw()
	{
		m_standartMaterial->position = m_position;
		GraphicsDevice::Draw(m_sprite);

		m_lifeTime -= Time::DeltaTime();
	}
}

#endif
