#pragma once
#include <Ace/SpriteSheet.h>
#include <Ace/Sprite.h>
#include <Ace/Transform.h>



namespace ace
{
	class Particle final
	{
		struct ParticleData final
		{
			Vector3 position;
			Vector3 velocity;
			UInt32 ID;
            float lifeTime;
			bool isAlive = true;
            
            ParticleData(
				const Vector3& position,
				const Vector3& velocity,
				const UInt32 id,
				const float lifeTime
			);
            
			friend bool operator<(const ParticleData& lhs, const ParticleData& rhs)
			{
				return lhs.lifeTime < rhs.lifeTime;
			}

			mutable Vector3 originalPosition;
			mutable float originalLifeTime;
		};

		struct SpriteData final
		{
			Sprite sprite;
            const UInt32 ID;
            
            SpriteData(const Sprite& sprite, const UInt32 id);

		};

	public:

		Particle();

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
		@param[in] velocity Vector3 veloctiy where particle is heading
		*/
		Particle(const Sprite& sprite, const float lifeTime, const Vector3& position, const Vector3& velocity);
		Particle(const SpriteSheet& spriteSheet, const float lifeTime, const Vector3& position, const Vector3& velocity);
		~Particle();

		void Draw();
		void Reset();
		void Update();

		inline bool IsAlive() const 
		{
			return m_isAlive;
		};

	private:
		std::vector<ParticleData> m_particles;
		std::vector<SpriteData> m_sprites;

		IndexBuffer m_indexBuffer;


		SpriteSheet m_sheet;
		bool m_isAlive;

	};


}


