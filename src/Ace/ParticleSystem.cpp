#include <Ace/ParticleSystem.h>


namespace ace
{
	ParticleSystem::ParticleSystem(
		const Particle& particle, 
		const Vector3& position, 
		const bool isActive
	) :
		m_particle(),
		m_position(position),
		m_isActive(isActive)
	{
		m_particles.emplace_back(particle);
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::Update()
	{
		if (m_isActive)
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
		m_isActive = !m_isActive;
	}

	bool ParticleSystem::GetIsActive()
	{
		return m_isActive;
	}

	void ParticleSystem::Draw()
	{
		for (UInt32 i = 0u; i < m_particles.size(); ++i)
		{
			if (m_particles[i].IsAlive())
			{
				m_particles[i].Update();
				m_particles[i].Draw();
			}
		}
	}

	void ParticleSystem::AddParticle(const Particle& newParticle)
	{
		m_particles.emplace_back(newParticle);
	}
}

