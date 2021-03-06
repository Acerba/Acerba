#pragma once

#include <Ace/AABB.h>
#include <Ace/CollisionData.h>
#include <Ace/IntTypes.h>
#include <Ace/Matrix2.h>
#include <Ace/Vector2.h>

#include <vector> // std::vector

namespace ace
{

    struct Collidable;
    
    struct CollidableImpl final
    {
        using Matrix2 = math::Matrix2;
        using Vector2 = math::Vector2;

        /**
            @brief Ctor.
        */
        CollidableImpl(const Vector2& position, const Matrix2& rotation);

        CollidableImpl& operator=(const CollidableImpl& other);

        /**
            @brief Dtor.
        */
        ~CollidableImpl();
        
        /**
            @brief Adds a collision to this collidableimpl with other. Does not add the collision to other.
            @param[in] other CollidableImpl.
        */
        void AddCollision(CollidableImpl* other, const Vector2& normal)
        {
            if (other)
            {
                m_collisions.emplace_back(other->m_owner, normal);
            }
        }

        /**
            @brief Destroy this CollidableImpl. Clears all collisions from all CollidableImpls.
        */
        void Destroy() const;

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
            m_hasChanged = true;
            return m_aabb;
        }

        /**
            @brief Retrieve the number of collisions this object has.
        */
        UInt32 GetCollisionCount() const
        {
            return m_collisions.size();
        }

        /**
            @brief Retrieve the objects which this collides with.
        */
        const std::vector<CollisionData>& GetCollisions() const
        {
            return m_collisions;
        }

        /**
            @brief Retrieve the unique ID of this.
            @return UInt32.
        */
        UInt32 GetID() const;

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
            m_hasChanged = true;
            return m_position;
        }

        /**
            @brief Retrieve the owner of this.
            @return Collidable.
        */
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
            m_hasChanged = true;
            return m_rotation;
        }

        inline bool HasChanged() const {
            return m_hasChanged;
        }

        /**
            @brief Check if the collidable has a marked collision with the other collidable.
            Does not actually check for collision, only if a collision has been marked previously.
            @param[in] id ID of the other collidable.
            @return True if the collidables are marked as colliding.
        */
        bool HasCollision(const UInt32 id) const;

        /**
            @brief Access the owner.
        */
        inline Collidable& operator*()
        {
            return *m_owner;
        }

        /**
            @brief Resets the collisions this collidable has received.
            Does not reset the collisions on the other collidable of the collision.
        */
        void ResetCollisions()
        {
            m_collisions.clear();
        }

        inline void SetHasChanged(const bool value)
        {
            m_hasChanged = value;
        }
        
        /**
            @brief Set the owner of this.
            @warning Only call this method in a Derived-from-Collidable ctor.
            @param[in] owner Collidable.
        */
        void SetOwner(Collidable* owner);

        
    private:
        
        std::vector<CollisionData> m_collisions;
        AABB m_aabb;
        Matrix2 m_rotation;
        Vector2 m_position;
        Collidable* m_owner;
        bool m_hasChanged;
        
    };
    
}
