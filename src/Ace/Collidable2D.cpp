#include <Ace/Collidable2D.h>
#include <Ace/Math.h>

namespace ace
{

    bool IsInTriangle(
        const Vector2& p,
        const Vector2& a,
        const Vector2& b,
        const Vector2& c
    )
    {
        const Vector2 v0 = c - a;
        const Vector2 v1 = b - a;
        const Vector2 v2 = p - a;

        const float d00 = Vector2::Dot(v0, v0);
        const float d01 = Vector2::Dot(v0, v1);
        const float d02 = Vector2::Dot(v0, v2);
        const float d11 = Vector2::Dot(v1, v1);
        const float d12 = Vector2::Dot(v1, v2);

        const float id = 1.f / (d00 * d11 - d01 * d01); // TODO: add div-by-zero check?
        const float u = (d11 * d02 - d01 * d12) * id;
        const float v = (d00 * d12 - d01 * d02) * id;
        return (u >= 0.f) && (v >= 0.f) && ((u + v) < 1.f);
    }


    Collidable2D::Collidable2D(const Vector2& position) :
        m_position(position)
        // m_rotation()
    {

    }

    Collidable2D::~Collidable2D()
    {

    }

    // Same

    // C-C
    template<> bool Collidable2D::IsColliding<Circle, Circle>(const Circle& a, const Circle& b)
    {
        return false;
    }
    // R-R
    template<> bool Collidable2D::IsColliding<Rectangle, Rectangle>(const Rectangle& a, const Rectangle& b)
    {
        return false;
    }
    // T-T
    template<> bool Collidable2D::IsColliding<Triangle, Triangle>(const Triangle& a, const Triangle& b)
    {
        return false;
    }

    // Different

    // C-R
    template<> bool Collidable2D::IsColliding<Circle, Rectangle>(const Circle& a, const Rectangle& b)
    {
        return false;
    }
    template<> bool Collidable2D::IsColliding<Rectangle, Circle>(const Rectangle& a, const Circle& b)
    {
        return IsColliding(b, a);
    }

    // C-T
    template<> bool Collidable2D::IsColliding<Circle, Triangle>(const Circle& a, const Triangle& b)
    {
        return false;
    }
    template<> bool Collidable2D::IsColliding<Triangle, Circle>(const Triangle& a, const Circle& b)
    {
        return IsColliding(b, a);
    }

    // R-T
    template<> bool Collidable2D::IsColliding<Rectangle, Triangle>(const Rectangle& a, const Triangle& b)
    {
        return false;
    }
    template<> bool Collidable2D::IsColliding<Triangle, Rectangle>(const Triangle& a, const Rectangle& b)
    {
        return IsColliding(b, a);
    }

    Circle::Circle(const Vector2& position, const float radius) :
        Collidable2D(position), m_radius(radius)
    {

    }

    bool Circle::IsColliding(const Vector2& point) const
    {
        const Vector2 d = point - m_position;
        return (d.x*d.x + d.y*d.y) <= (m_radius * m_radius);
    }


    Rectangle::Rectangle(const Vector2& position, const Vector2& extents) :
        Collidable2D(position), m_extents(extents)
    {

    }

    bool Rectangle::IsColliding(const Vector2& point) const
    {
        const Vector2 pos = m_position + m_extents;
        const Vector2 neg = m_position + m_extents.Inverse();
        return IsInTriangle(
            point,
            pos,
            m_position + Vector2(-m_extents.x, m_extents.y),
            neg
        ) || IsInTriangle(
            point,
            neg,
            m_position + Vector2(m_extents.x, -m_extents.y),
            pos
        );
    }


    Triangle::Triangle(const Vector2& position, const Vector2 (&extents)[3u]) :
        Collidable2D(position), m_extents(extents)
    {

    }

    bool Triangle::IsColliding(const Vector2& point) const
    {
        return IsInTriangle(
            point,
            m_position + m_extents[0],
            m_position + m_extents[1],
            m_position + m_extents[2]
        );
    }

}