#include <Ace/AABB.h>
#include <Ace/Collidable.h>

#include <limits>

namespace ace
{

    AABB::AABB(const Vector2& min, const Vector2& max) : min(min), max(max) { }

    void AABB::Update(const Collidable& c)
    {
        min = DefaultMin();
        max = DefaultMax();
        for (const auto& vertex : c.GetVertices())
        {
            if (vertex.x < min.x) min.x = vertex.x;
            else if (max.x < vertex.x) max.x = vertex.x;
            if (vertex.y < min.y) min.y = vertex.y;
            else if (max.y < vertex.y) max.y = vertex.y;
        }
    }

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
    
    Vector2 AABB::DefaultMin()
    {
        return { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    }
    Vector2 AABB::DefaultMax()
    {
        return { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
    }

}