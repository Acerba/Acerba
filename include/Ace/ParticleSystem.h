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
		ParticleSystem(const Particle& particle, const Vector3& position, const bool isActive);
		~ParticleSystem();

		void Update();
		void Draw();
		void ToggleActive();

		Vector3 m_position;
		Particle m_particle;
		bool isActive;

		StandardMaterial m_standardMaterial;
	};
}

