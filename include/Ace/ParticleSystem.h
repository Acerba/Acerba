#pragma once

#include <Ace/Transform.h>
#include <Ace/Particle.h>
#include <Ace/StandardMaterial.h>


namespace ace
{
	class ParticleSystem
	{
	public:

		ParticleSystem() {};
		ParticleSystem(const Particle& particle, const Vector3& position, const bool isActive = true);
		~ParticleSystem();

		void Update();
		void Draw();
		void ToggleActive();
		void AddParticle(const Particle& newParticle);
		bool GetIsActive();
		inline Material& GetMaterial()
		{
			return m_standardMaterial;
		};

	private:

		Vector3 m_position;
		Particle m_particle;
		bool m_isActive;

		std::vector<Particle> m_particles;

		StandardMaterial m_standardMaterial;
	};
}

