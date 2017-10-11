#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Types.h>

#include <vector>

namespace ace
{
    class Circle;
    class Rectangle;
    class Triangle;

    class Collidable2D
    {
    public:
        // TEMP find && replace once finished
        using CVT = Vector2;
    protected:
        // Quaternion m_rotation;
        CVT m_position;
    public:

        enum class Axis : UInt8
        {
            X = 0, Pitch = 0,
            Y = 1, Yaw = 1,
            Z = 2, Roll = 2
        };

        Collidable2D(const CVT& position);
        virtual ~Collidable2D() = 0;

        /**
            @brief Checks if the point is in or on the Collidable.
            @param[in] point Point in same coordinate system as the Collidable to check.
            @return True if the point is inside or touching the Collidable.
        */
        virtual bool IsColliding(const CVT& point) const = 0;

        /**
            @brief Checks if the Collidables are touching.
            @param[in] a An object derived from Collidable2D.
            @param[in] b An object derived from Collidable2D.
            @return True if the Collidables are touching or overlapping.
        */
        static bool IsColliding(const Collidable2D& a, const Collidable2D& b);

        inline const CVT& GetPosition() const
        {
            return m_position;
        }
        inline CVT& GetPosition()
        {
            return m_position;
        }

        virtual std::vector<CVT> GetVertices() const = 0;

        // virtual void Rotate()
    };

    class Circle final : public Collidable2D
    {
        float m_radius;
    public:
        Circle(const CVT& position, const float radius);
        bool IsColliding(const CVT& point) const override;

        inline float GetRadius() const
        {
            return m_radius;
        }

        std::vector<CVT> GetVertices() const final override;
    };

    class Rectangle final : public Collidable2D
    {
        CVT m_extents;
    public:
        Rectangle(const CVT& position, const CVT& extents);
        bool IsColliding(const CVT& point) const override;

        inline const CVT& GetExtents() const
        {
            return m_extents;
        }

        std::vector<CVT> GetVertices() const final override;
    };

    class Triangle final : public Collidable2D
    {
        CVT m_extents[3u];
    public:
        Triangle(const CVT& position, const CVT (&extents)[3u]);
        bool IsColliding(const CVT& point) const override;

        inline const CVT(&GetExtents() const)[3u]
        {
            return m_extents;
        }

        std::vector<CVT> GetVertices() const final override;
    };

}