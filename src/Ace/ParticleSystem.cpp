#include <Ace/ParticleSystem.h>

namespace ace
{
	ParticleSystem::ParticleSystem(Particle& particle, Vector3 position)
	{
		m_particle = particle;
		m_position = position;
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::Update()
	{
		if (isActive)
		{
			Draw();
		}
		else
		{
			//TODO: Jotakin tänne?
			printf("ParticleSystem isn't active");
		}
	}

	void ParticleSystem::ToggleActive()
	{
		isActive != isActive;
	}

	void ParticleSystem::Draw()
	{
		m_particle.Draw();
		if (m_particle.m_lifeTime <= 0)
		{
			isActive = false;;
		}
	}
}