#pragma once

#include <Ace/Types.h>

namespace ace
{
    class Circle;
    class Rectangle;
    class Triangle;

    class Collidable2D
    {
    protected:
        // Quaternion m_rotation;
        Vector2 m_position;
    public:
        Collidable2D(const Vector2& position);
        virtual ~Collidable2D() = 0;

        /**
        @brief Checks if the point is in or on the Collidable.
        @param[in] point Point in same coordinate system as the Collidable to check.
        @return True if the point is inside or touching the Collidable.
        */
        virtual bool IsColliding(const Vector2& point) const = 0;

        /**
        @brief Checks if the Collidables are touching.
        @param[in] a An object derived from Collidable2D.
        @param[in] b An object derived from Collidable2D.
        @warning Requires specializations if using custom Collidables.
        @return True if the Collidables are touching or overlapping.
        */
        template <typename A, typename B>
        static bool IsColliding(const A& a, const B& b);

        inline const Vector2& GetPosition() const
        {
            return m_position;
        }

        // virtual void Rotate()
    };

    class Circle : public Collidable2D
    {
        float m_radius;
    public:
        Circle(const Vector2& position, const float radius);
        bool IsColliding(const Vector2& point) const override;

        inline float GetRadius() const
        {
            return m_radius;
        }
    };

    class Rectangle : public Collidable2D
    {
        Vector2 m_extents;
    public:
        Rectangle(const Vector2& position, const Vector2& extents);
        bool IsColliding(const Vector2& point) const override;
    };

    class Triangle : public Collidable2D
    {
        Vector2 m_extents[3u];
    public:
        Triangle(const Vector2& position, const Vector2 (&extents)[3u]);
        bool IsColliding(const Vector2& point) const override;
    };

}