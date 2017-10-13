#pragma once

#include <Ace/Vector2.h>

namespace ace
{

    struct AABB;
    using math::Vector2;
    
    struct Raycast final
    {
        struct RayInfo final
        {
            Vector2 hitPosition;
            float lengthIn;
            bool hasHit;

            RayInfo();

            inline operator bool()
            {
                return hasHit;
            }
        };


        Raycast(const Vector2& start, const Vector2& ray);
        Raycast(const Vector2& start, const Vector2& direction, const float length);

        static bool IsColliding(const Raycast& ray, const AABB& aabb);

        // inline const RayInfo& GetRayInfo() const
        // {
        //     return m_rayInfo;
        // }

        const Vector2 start;
        const Vector2 unitDirection;
        const Vector2 invDirection;
        // const float length;
    private:
        // RayInfo m_rayInfo;
    };


}