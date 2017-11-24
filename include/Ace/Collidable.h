#pragma once

#include <Ace/CollisionData.h>
#include <Ace/IntTypes.h>
#include <Ace/Matrix2.h>
#include <Ace/Vector2.h>

#include <vector> // std::vector

namespace ace
{
    struct AABB;
    struct CollidableImpl;

    struct Collidable
    {
        using Matrix2 = math::Matrix2;
        using Vector2 = math::Vector2;

        struct Mask final
        {
            enum : UInt8
            {
                A = 1<<0,
                B = 1<<1,
                C = 1<<2,
                D = 1<<3,
                E = 1<<4,
                F = 1<<5,
                G = 1<<6,
                H = 1<<7
            };
            Mask() = delete;
        };

        /**
            @brief Ctor.
            @param[in] position Position of the collidable.
            @param[in] rotation Rotation of the collidable.
        */
        Collidable(
            const Vector2& position,
            const Matrix2& rotation = math::s_identity2
        );

        Collidable& operator=(const Collidable& other);

        /**
            @brief Dtor. Clears all collisions from all Collidables.
        */
        virtual ~Collidable() = 0;

        /**
            @brief Retrieve the AABB of this collidable.
            @return AABB
        */
        const AABB& GetAABB() const;

        /**
            @brief Retrieve the number of collidables this one is colliding with.
            @return UInt32
        */
        UInt32 GetCollisionCount() const;

        /**
            @brief Retrieve CollisionData from each collision happening with this Collidable.
            @return vector of CollisionData
        */
        const std::vector<CollisionData>& GetCollisions() const;

        /**
            @brief Retrieve the id of the collidable.
            @return ID.
        */
        inline UInt32 GetID() const
        {
            return m_id;
        }

        // Vector2 GetGlobalPosition() const;

        /**
            @brief Retrieve the local position of the collidable.
            @return Local position.
        */
        const Vector2& GetLocalPosition() const;
        Vector2& GetLocalPosition();

        /**
            @brief Retrieve the current mask of the collidable.
            @return UInt8
         */
        inline UInt8 GetMask() const
        {
            return m_mask;
        }

        /**
            @brief Retrieve all normals of this Collidable. Empty if this is a Circle.
        */
        std::vector<Vector2> GetNormals() const;

        /**
            @brief Retrieve the current rotation of the collidable.
            @return Rotation.
         */
        const Matrix2& GetRotation() const;
        Matrix2& GetRotation();


        /**
            @return Global vertices of the collidable.
        */
        virtual std::vector<Vector2> GetVertices() const = 0;

        bool HasChanged() const;

        /**
            @brief Check if the collidable has a marked collision with the other collidable.
            Does not actually check for collision, only if a collision has been marked previously.
            @param[in] other Other collidable.
            @return True if the collidables are marked as colliding.
        */
        bool HasCollision(const Collidable& other) const;

        /**
            @brief Check if the collidable has atleast some same masks as the param, so that they can collide.
            @param[in] mask Other mask to test against.
            @return True if the collidable can collide with the param mask.
         */
        inline bool HasMask(const UInt8 mask) const
        {
            return (m_mask & mask);
        }
        /**
            @brief Check if the collidables have atleast some same masks on, so that they can collide.
            @param[in] other Other collidable to test against.
            @return True if the collidables are allowed to collide.
        */
        inline bool HasMask(const Collidable& other) const
        {
            return HasMask(other.m_mask);
        }

        /**
            @brief Checks if the point is in or on the Collidable.
            @param[in] point Point in same coordinate system as the Collidable to check.
            @return True if the point is inside or touching the Collidable.
        */
        virtual bool IsColliding(const Vector2& point) const = 0;

        /**
            @brief Checks if the Collidables are touching.
            @param[in] a An object derived from Collidable.
            @param[in] b An object derived from Collidable.
            @return True if the Collidables are touching or overlapping.
        */
        static bool IsColliding(const Collidable& a, const Collidable& b);

        /**
            @brief Access the underlying implementation.
            @return CollidableImpl.
        */
        inline const CollidableImpl& operator*() const
        {
            return m_impl;
        }
        inline CollidableImpl& operator*()
        {
            return m_impl;
        }

        /**
            @brief Reserve memory for Collidables.
            @param[in] size Number of Collidables to support.
        */
        static void Reserve(const UInt32 size);

        /**
            @brief Reset collisions regarding this Collidable.
            Does not remove markings from other collidables with this one.
        */
        void ResetCollisions();

        /**
            @brief Rotate the collidables vertices around its center by deg degrees.
            Modifies the objects vertices and resets the rotation, making the new orientation stay on until another call to this method.
            @param[in] deg Degrees to rotate.
        */
        virtual void Rotate(float deg) = 0;

        void SetHasChanged(const bool value);

        /**
            @brief Set collision mask for this collidable. Collidables which have atleast partly same mask can collide with each other.
            @param[in] mask New masking value to set. Preferably from Mask::
            @see Mask
         */
        void SetMask(const UInt8 mask);

        virtual void UpdateAABB() = 0;

        /**
            @brief Update collisions regarding this Collidable. Make sure you have called BVH::Update() beforehand.
            Also marks collisions on to the other collidables, if any.
        */
        void UpdateCollisions();

        
        
    protected:
        CollidableImpl& m_impl;
        const UInt32 m_id;
        UInt8 m_mask;

    }; // Collidable


    struct Circle final : public Collidable
    {
        using Matrix2 = Collidable::Matrix2;
        using Vector2 = Collidable::Vector2;
        
        Circle(const float radius, const Vector2& position, const Matrix2& rotation = math::s_identity2);

        inline float GetRadius() const
        {
            return m_radius;
        }

        std::vector<Vector2> GetVertices() const final override;

        bool IsColliding(const Vector2& point) const override;
        
        void Rotate(float deg) final override;

        void UpdateAABB() final override;

    private:

        float m_radius;

    }; // Circle


    struct Rectangle final : public Collidable
    {
        using Matrix2 = Collidable::Matrix2;
        using Vector2 = Collidable::Vector2;

        Rectangle(const Vector2& extents, const Vector2& position, const Matrix2& rotation = math::s_identity2);

        Rectangle(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d);

        std::vector<Vector2> GetVertices() const final override;

        bool IsColliding(const Vector2& point) const override;

        void Rotate(float deg) final override;

        void UpdateAABB() final override;

    private:

        // Global points, don't care about center
        Vector2 m_points[4u];

    }; // Rectangle


    struct Triangle final : public Collidable
    {
        using Matrix2 = Collidable::Matrix2;
        using Vector2 = Collidable::Vector2;

        Triangle(const Vector2 (&extents)[3u], const Vector2& position, const Matrix2& rotation = math::s_identity2);

        Triangle(const Vector2& a, const Vector2& b, const Vector2& c);

        std::vector<Vector2> GetVertices() const final override;

        bool IsColliding(const Vector2& point) const override;

        void Rotate(float deg) final override;

        void UpdateAABB() final override;

    private:

        // Global points, don't care about center
        Vector2 m_points[3u];

    }; // Triangle

}