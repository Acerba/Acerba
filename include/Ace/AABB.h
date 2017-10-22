#pragma once

#include <Ace/Vector2.h>

namespace ace
{
    struct Collidable;
    
    struct AABB final
    {
        using Vector2 = math::Vector2;

        Vector2 min;
        Vector2 max;

        struct Split;
        
        AABB(
            const Vector2& min = DefaultMin(),
            const Vector2& max = DefaultMax()
        );

        Vector2 GetCenter() const;

        static bool IsColliding(const AABB& a, const AABB& b);
        
        static bool IsColliding(const AABB& a, const Vector2& point);

        void Merge(const AABB& other);
        
        void Reset();
        
        void Update(const Vector2& vertex);
        
    private:
        static const Vector2& DefaultMin();
        static const Vector2& DefaultMax();
        
    };

    struct AABB::Split final
    {
        AABB leftTop;
        AABB rightTop;
        AABB leftBottom;
        AABB rightBottom;

        Split(const AABB& original);
    };
}