#include <Ace/AABB.h>

#include <limits> // std::numeric_limits

namespace ace
{
    using Vector2 = AABB::Vector2;

    static const Vector2 s_defaultMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    static const Vector2 s_defaultMax(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());


    AABB::Split2::Split2(const AABB& original) :
        left(),
        right()
    {
        const float centerX = (original.min.x + original.max.x) * 0.5f;

        left.min = original.min;
        left.max.x = centerX;
        left.max.y = original.max.y;

        right.min.x = centerX;
        right.min.y = original.min.y;
        right.max = original.max;
    }

    AABB::Split4::Split4(const AABB& original) :
        leftTop(),
        rightTop(),
        leftBottom(),
        rightBottom()
    {
        const Vector2 center(original.GetCenter());

        leftTop.min.x = original.min.x;
        leftTop.min.y = center.y;
        leftTop.max.x = center.x;
        leftTop.max.y = original.max.y;

        rightTop.min = center;
        rightTop.max = original.max;

        leftBottom.min = original.min;
        leftBottom.max = center;

        rightBottom.min.x = center.x;
        rightBottom.min.y = original.min.y;
        rightBottom.max.x = original.max.x;
        rightBottom.max.y = center.y;
    }

    AABB::AABB(const Vector2& min, const Vector2& max) : min(min), max(max) { }

    Vector2 AABB::GetCenter() const
    {
        return (min + max) * 0.5f;
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

    void AABB::Merge(const Vector2& point)
    {
        if (point.x < min.x) min.x = point.x;
        else if (max.x < point.x) max.x = point.x;
        if (point.y < min.y) min.y = point.y;
        else if (max.y < point.y) max.y = point.y;
    }

    void AABB::Merge(const AABB& other)
    {
        Merge(other.min);
        Merge(other.max);
    }

    void AABB::Reset()
    {
        min = s_defaultMin;
        max = s_defaultMax;
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