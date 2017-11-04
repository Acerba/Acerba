#include <Ace/ParticleSystem.h>


namespace ace
{
	ParticleSystem::ParticleSystem(Particle& particle, Vector3 position, bool isActiv = false)
	{
		// m_particle = particle; // TODO: Disabled for compilation error
		m_position = position;
		isActive = isActiv;
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
			//TODO: Jotakin tanne?
			printf("ParticleSystem isn't active");
		}
	}

	void ParticleSystem::ToggleActive()
	{
		isActive = !isActive;
	}

	void ParticleSystem::Draw()
	{
		m_particle.Draw();

	}
}

