#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Matrix2.h>
#include <Ace/Vector2.h>

#include <Ace/BVH.h>

#include <vector>

namespace ace
{
    struct AABB;

    struct Collidable
    {
        using Matrix2 = math::Matrix2;
        using Vector2 = math::Vector2;

        Collidable(
            BVH::CollidableID id,
            const Vector2& position,
            const Matrix2& rotation = math::s_identity2
        );
        virtual ~Collidable() = 0;

        /**
            @brief Retrieve the AABB of this collidable.
            @return AABB
        */
        const AABB& GetAABB() const;

        /**
            @brief Retrieve the id of the collidable.
            @return ID.
         */
        virtual UInt32 GetID() const = 0;


        // Vector2 GetGlobalPosition() const;

        /**
            @brief Retrieve the local position of the collidable.
            @return Local position.
         */
        const Vector2& GetLocalPosition() const;
        Vector2& GetLocalPosition();

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

        /**
            @brief Update collisions regarding this Collidable. Make sure you have called BVH::Update() beforehand.
            Also marks collisions on to the other collidables, if any.
        */
        void UpdateCollisions();

    protected:

        virtual void UpdateAABB(const bool accountRotation = true) = 0;

        CollidableImpl& m_impl;

        BVH::CollidableID m_id;
    }; // Collidable


    struct Circle final : public Collidable
    {
        using Matrix2 = Collidable::Matrix2;
        using Vector2 = Collidable::Vector2;
        
        Circle(const float radius, const Vector2& position, const Matrix2& rotation = math::s_identity2);


        UInt32 GetID() const final override
        {
            return m_id.GetID<Circle>();
        }

        inline float GetRadius() const
        {
            return m_radius;
        }

        std::vector<Vector2> GetVertices() const final override;

        bool IsColliding(const Vector2& point) const override;
        
        void Rotate(float deg) final override;

    protected:

        void UpdateAABB(const bool accountRotation = true) final override;

    private:

        float m_radius;

    }; // Circle


    struct Rectangle final : public Collidable
    {
        using Matrix2 = Collidable::Matrix2;
        using Vector2 = Collidable::Vector2;

        Rectangle(const Vector2& extents, const Vector2& position, const Matrix2& rotation = math::s_identity2);

        inline const Vector2& GetExtents() const
        {
            return m_extents;
        }

        UInt32 GetID() const final override
        {
            return m_id.GetID<Rectangle>();
        }
        
        std::vector<Vector2> GetVertices() const final override;

        bool IsColliding(const Vector2& point) const override;

        void Rotate(float deg) final override;

    protected:

        void UpdateAABB(const bool accountRotation = true) final override;

    private:

        Vector2 m_extents;

    }; // Rectangle


    struct Triangle final : public Collidable
    {
        using Matrix2 = Collidable::Matrix2;
        using Vector2 = Collidable::Vector2;

        Triangle(const Vector2 (&extents)[3u], const Vector2& position, const Matrix2& rotation = math::s_identity2);

        inline const Vector2(&GetExtents() const)[3u]
        {
            return m_extents;
        }

        UInt32 GetID() const final override
        {
            return m_id.GetID<Triangle>();
        }
        
        std::vector<Vector2> GetVertices() const final override;

        bool IsColliding(const Vector2& point) const override;

        void Rotate(float deg) final override;

    protected:

        void UpdateAABB(const bool accountRotation = true) final override;

    private:

        Vector2 m_extents[3u];

    }; // Triangle

}