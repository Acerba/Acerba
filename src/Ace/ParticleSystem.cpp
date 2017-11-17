#include <Ace/ParticleSystem.h>


namespace ace
{
	ParticleSystem::ParticleSystem(
		const Particle& particle, 
		const Vector3& position, 
		const bool isActiv = false
	) :
		m_particle(particle),
		m_position(position),
		isActive(isActiv)
	{

	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::Update()
	{
		if (isActive)
		{
			GraphicsDevice::SetMaterial(m_standardMaterial);
			Draw();
		}
		else
		{
			//TODO: Jotakin tanne? -> error message
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

