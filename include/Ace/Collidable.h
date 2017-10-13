#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Matrix2.h>
#include <Ace/Vector2.h>

#include <vector>

namespace ace
{
    using math::Matrix2;
    using math::Vector2;

    class Collidable
    {
    protected:
        Matrix2 m_rotation;
        Vector2 m_position;
    public:

        Collidable(const Vector2& position, const Matrix2& rotation = Matrix2::Identity());
        virtual ~Collidable() = 0;

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

        inline Vector2 GetGlobalPosition() const
        {
            return m_rotation * m_position;
        }
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



        virtual std::vector<Vector2> GetVertices() const = 0;

        void Rotate(const float deg);
    };

    class Circle final : public Collidable
    {
        float m_radius;
    public:
        Circle(const float radius, const Vector2& position, const Matrix2& rotation = Matrix2::Identity());
        bool IsColliding(const Vector2& point) const override;

        inline float GetRadius() const
        {
            return m_radius;
        }

        std::vector<Vector2> GetVertices() const final override;
    };

    class Rectangle final : public Collidable
    {
        Vector2 m_extents;
    public:
        Rectangle(const Vector2& extents, const Vector2& position, const Matrix2& rotation = Matrix2::Identity());
        bool IsColliding(const Vector2& point) const override;

        inline const Vector2& GetExtents() const
        {
            return m_extents;
        }

        std::vector<Vector2> GetVertices() const final override;
    };

    class Triangle final : public Collidable
    {
        Vector2 m_extents[3u];
    public:
        Triangle(const Vector2 (&extents)[3u], const Vector2& position, const Matrix2& rotation = Matrix2::Identity());
        bool IsColliding(const Vector2& point) const override;

        inline const Vector2(&GetExtents() const)[3u]
        {
            return m_extents;
        }

        std::vector<Vector2> GetVertices() const final override;
    };

}