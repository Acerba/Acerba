#include <Ace/Raycast.h>
#include <Ace/AABB.h>
#include <Ace/Math.h>

namespace ace
{

    struct MinMax
    {
        MinMax(float a, float b) : min(math::Min(a, b)), max(math::Max(a, b)) { }
        const float min;
        const float max;
    };

    Raycast::Raycast(const Vector2& start, const Vector2& ray) :
        start(start),
        unitDirection(ray.Normalize()),
        invDirection(unitDirection.Invert())
        // length(ray.Length())
    {

    }

    Raycast::Raycast(const Vector2& start, const Vector2& direction, const float length) :
        start(start),
        unitDirection(direction.Normalize()),
        invDirection(unitDirection.Invert())
        //length(math::Abs(length))
    {

    }

    Raycast::RayInfo::RayInfo() :
        hitPosition(), lengthIn(0.f), hasHit(false)
    {

    }

    bool Raycast::IsColliding(const Raycast& ray, const AABB& aabb)
    {
        const MinMax x(
            (aabb.min.x - ray.start.x) * ray.invDirection.x,
            (aabb.max.x - ray.start.x) * ray.invDirection.x
        );
        const MinMax y(
            (aabb.min.y - ray.start.y) * ray.invDirection.y,
            (aabb.max.y - ray.start.y) * ray.invDirection.y
        );
        return math::Min(x.max, y.max) >= math::Max(x.min, y.min);
    }
}