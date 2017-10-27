#pragma once
#include <Ace/SpriteSheet.h>
#include <Ace/Sprite.h>
#include <Ace/Transform.h>
#include <Ace/StandardMaterial.h>

namespace ace
{
	class Particle
	{
	public:

		Particle() {};

		/**
			@brief Create particle
			@param[in] SpriteSheet SpriteSheet which contains sprites
			@param[in] lifeTime Time how long sprite will live
			@param[in] position Vector3 position where particle will be
		*/
		//Particle(const SpriteSheet& sheet, float lifeTime, Vector3 position);

		/**
		@brief Create particle
		@param[in] Sprite Sprite image
		@param[in] lifeTime Time how long sprite will live
		@param[in] position Vector3 position where particle will be
		*/
		Particle(const Sprite& sprite, float lifeTime, Vector3 position);
		~Particle();

		void Draw();

		float m_lifeTime;
		Vector3 m_position;

		//Private?
		Sprite m_sprite;
		SpriteSheet m_sheet;

		StandardMaterial m_standartMaterial;
		
	};
}