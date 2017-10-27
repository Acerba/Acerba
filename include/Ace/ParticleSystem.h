#pragma once
#include <Ace/Transform.h>
#include <Ace/Particle.h>

namespace ace
{
	class ParticleSystem
	{
	public:

		ParticleSystem() {};
		ParticleSystem(Particle& particle, Vector3 position);
		~ParticleSystem();

		void Update();
		void Draw();
		void ToggleActive();

		Vector3 m_position;
		Particle m_particle;
		bool isActive;
	};
}