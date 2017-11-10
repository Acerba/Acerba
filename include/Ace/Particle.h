#pragma once
#include <Ace/SpriteSheet.h>
#include <Ace/Sprite.h>
#include <Ace/Transform.h>



namespace ace
{
	class Particle final
	{
	public:

		struct ParticleData final
		{
			Vector3 position;
			Vector3 velocity;
			UInt32 ID;
            float lifeTime;
            
            ParticleData(const Vector3& position, const Vector3& velocity, const float lifeTime);
            
			friend bool operator<(const ParticleData& lhs, const ParticleData& rhs)
			{
				return lhs.lifeTime < rhs.lifeTime;
			}
		};

		struct SpriteData final
		{
			Sprite sprite;
            const UInt32 ID;
            
            SpriteData();
		};

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
		~Particle();

		void Draw();
		void Update();

		std::vector<ParticleData> particles;
		std::vector<Sprite> sprites;

		IndexBuffer indexBuffer;

		//Private?
		SpriteSheet m_sheet;

		
	};


}


