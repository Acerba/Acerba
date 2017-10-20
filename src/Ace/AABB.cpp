#include <Ace/AABB.h>
#include <Ace/Collidable.h>

#include <limits>

namespace ace
{
    using Vector2 = AABB::Vector2;

    static const Vector2 s_defaultMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    static const Vector2 s_defaultMax(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    AABB::AABB(const Vector2& min, const Vector2& max) : min(min), max(max) { }

    bool AABB::IsColliding(const AABB& a, const AABB& b)
    {
        return
            a.max.x >= b.min.x &&
            a.min.x <= b.max.x &&
            a.max.y >= b.min.y &&
            a.min.y <= b.max.y;
    }

    bool AABB::IsColliding(const AABB& a, const Vector2& point)
    {
        return
            a.min.x <= point.x && point.x <= a.max.x &&
            a.min.y <= point.y && point.y <= a.max.y;
    }

    void AABB::Merge(const AABB& other)
    {
        Update(other.min);
        Update(other.max);
    }

    void AABB::Reset()
    {
        min = s_defaultMin;
        max = s_defaultMax;
    }

    void AABB::Update(const Vector2& vertex)
    {
        if (vertex.x < min.x) min.x = vertex.x;
        else if (max.x < vertex.x) max.x = vertex.x;
        if (vertex.y < min.y) min.y = vertex.y;
        else if (max.y < vertex.y) max.y = vertex.y;
    }
    
    const Vector2& AABB::DefaultMin()
    {
        return s_defaultMin;
    }
    const Vector2& AABB::DefaultMax()
    {
        return s_defaultMax;
    }

}