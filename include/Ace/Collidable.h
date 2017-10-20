#pragma once

#include <Ace/AABB.h>
#include <Ace/IntTypes.h>
#include <Ace/Matrix2.h>
#include <Ace/Vector2.h>

#include <Ace/BVH.h>

#include <memory> // shared_from_this
#include <vector>

namespace ace
{


    struct Collidable : public std::enable_shared_from_this<Collidable>
    {
        using Vector2 = math::Vector2;
        using Matrix2 = math::Matrix2;

        Collidable(
            BVH::CollidableID id,
            const Vector2& position,
            const Matrix2& rotation = math::s_identity2
        );
        virtual ~Collidable() = 0;

        inline const AABB& GetAABB() const
        {
            return m_aabb;
        }

        virtual UInt32 GetID() const = 0;


        // Vector2 GetGlobalPosition() const;
        inline const Vector2& GetLocalPosition() const
        {
            return m_position;
        }
        inline Vector2& GetLocalPosition()
        {
            return m_position;
        }


        inline const Matrix2& GetRotation() const
        {
            return m_rotation;
        }
        inline Matrix2& GetRotation()
        {
            return m_rotation;
        }

        std::shared_ptr<Collidable> GetShared()
        {
            return this->shared_from_this();
        }


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
            @brief Rotate the collidables vertices around its center by deg degrees.
            Modifies the objects vertices and resets the rotation, making the new orientation stay on until another call to this method.
            @param[in] deg Degrees to rotate.
        */
        virtual void Rotate(float deg) = 0;

    protected:

        virtual void UpdateAABB(const bool accountRotation = true) = 0;

        AABB m_aabb;
        Matrix2 m_rotation;
        Vector2 m_position;
        BVH::CollidableID m_id;
    };


    struct Circle final : public Collidable
    {
        using Vector2 = Collidable::Vector2;
        using Matrix2 = Collidable::Matrix2;
        
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
        using Vector2 = Collidable::Vector2;
        using Matrix2 = Collidable::Matrix2;

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
        using Vector2 = Collidable::Vector2;
        using Matrix2 = Collidable::Matrix2;

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