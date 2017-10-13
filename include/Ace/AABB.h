#pragma once

#include <Ace/Vector2.h>

namespace ace
{
    using math::Vector2;

    struct Collidable;

    struct AABB final
    {

        Vector2 min;
        Vector2 max;

        AABB(
            const Vector2& min = DefaultMin(),
            const Vector2& max = DefaultMax()
        );

        void Update(const Collidable& c);

        static bool IsColliding(const AABB& a, const AABB& b);

        static bool IsColliding(const AABB& a, const Vector2& point);

    protected:
        static Vector2 DefaultMin();
        static Vector2 DefaultMax();

    };
}