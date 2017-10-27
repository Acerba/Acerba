#pragma once

#include <Ace/AABB.h>
#include <Ace/IntTypes.h>
#include <Ace/Matrix2.h>
#include <Ace/Vector2.h>

#include <memory> // std::shared_from_this
#include <unordered_set>

namespace ace
{
    
    
    struct CollidableImpl final : public std::enable_shared_from_this<CollidableImpl>
    {
        using Matrix2 = math::Matrix2;
        using Vector2 = math::Vector2;

        /**
            @brief Ctor.
            @warning Do not call manually. Use CreateCollidable instead.
            @see CreateCollidableImpl()
        */
        CollidableImpl(const Vector2& position, const Matrix2& rotation);

        ~CollidableImpl();
        
        /**
        @brief Adds a collision to this collidableimpl with other. Does not add the collision to other.
        @param[in] other CollidableImpl.
        */
        void AddCollision(CollidableImpl& other)
        {
            m_collisions.insert(other.GetShared());
        }
        
        static CollidableImpl& CreateCollidableImpl(const Vector2& position, const Matrix2& rotation);

        void Destroy();
        

        /**
            @brief Retrieve the AABB of this collidable.
            @return AABB
        */
        inline const AABB& GetAABB() const
        {
            return m_aabb;
        }
        inline AABB& GetAABB()
        {
            return m_aabb;
        }

        inline UInt32 GetID() const
        {
            return 0u; // TODO:
        }

        /**
            @brief Retrieve the local position of the collidable.
            @return Local position.
         */
        inline const Vector2& GetLocalPosition() const
        {
            return m_position;
        }
        inline Vector2& GetLocalPosition()
        {
            return m_position;
        }

        inline Collidable& GetOwner()
        {
            return *m_owner;
        }

        /**
            @brief Retrieve the current rotation of the collidable.
            @return Rotation.
         */
        inline const Matrix2& GetRotation() const
        {
            return m_rotation;
        }
        inline Matrix2& GetRotation()
        {
            return m_rotation;
        }

        
        std::shared_ptr<CollidableImpl> GetShared()
        {
            return this->shared_from_this();
        }

        /**
            @brief Reserve memory for collidables.
            @param[in] size Number of simultaneous collidables to support.
        */
        static void Reserve(const UInt32 size);
        
        /**
            @brief Resets the collisions this collidable has received.
            Does not reset the collisions on the other collidable of the collision.
        */
        void ResetCollisions()
        {
            m_collisions.clear();
        }
        
        void SetOwner(Collidable* owner)
        {
            m_owner = owner;
        }

        void UpdateCollisions();
        
        
        
    private:
        
        std::unordered_set<std::shared_ptr<CollidableImpl>> m_collisions;
        AABB m_aabb;
        Matrix2 m_rotation;
        Vector2 m_position;
        Collidable* m_owner;
        
    };
    
}
