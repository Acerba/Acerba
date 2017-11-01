#pragma once

#include <Ace/Vector2.h>

namespace ace
{
    struct AABB final
    {
        using Vector2 = math::Vector2;

        Vector2 min;
        Vector2 max;

        struct Split2;
        struct Split4;
        
        /**
            @brief Ctor.
            @param[in] min Minimum bounds. Defaults to maximum value so first merge always happens.
            @param[in] max Maximum bounds. Defaults to minimum value so first merge always happens.
         */
        AABB(
            const Vector2& min = DefaultMin(),
            const Vector2& max = DefaultMax()
        );

        /**
            @brief Retrieve the center point of the AABB.
            @return Vector2
        */
        Vector2 GetCenter() const;

        /**
            @brief Check if the AABBs are colliding.
            @param[in] a AABB 1
            @param[in] b AABB 2
            @return True if the AABBs are touching or overlapping.
        */
        static bool IsColliding(const AABB& a, const AABB& b);
        
        /**
            @brief Check if the AABB is colliding with a point.
            @param[in] a AABB.
            @param[in] point A point in space.
            @return True if the point is touching or overlapping the AABB.
        */
        static bool IsColliding(const AABB& a, const Vector2& point);

        /**
            @brief Extend this AABB to include the point if it already does not.
            @param[in] point A point in space to include.
        */
        void Merge(const Vector2& point);

        /**
            @brief Extend this AABB to include the other AABB if it already does not.
            @param[in] other An AABB to include.
        */
        void Merge(const AABB& other);
        
        /**
            @brief Sets min and max to DefaultMin and DefaultMax.
        */
        void Reset();
        
    private:

        /**
            @brief Retrieves maximum value so first merge always happens.
            @return Vector2
        */
        static const Vector2& DefaultMin();
        /**
            @brief Retrieves minimum value so first merge always happens.
            @return Vector2
        */
        static const Vector2& DefaultMax();
        
    };

    /**
        @brief Splits an AABB to left and right halves.
    */
    struct AABB::Split2 final
    {
        AABB left;
        AABB right;

        /**
            @brief Ctor.
            @param[in] original Original AABB to split in to left and right parts.
         */
        Split2(const AABB& original);
    };

    /**
        @brief Splits an AABB to four parts.
    */
    struct AABB::Split4 final
    {
        AABB leftTop;
        AABB rightTop;
        AABB leftBottom;
        AABB rightBottom;

        /**
            @brief Ctor.
            @param[in] original Original AABB to split into four parts.
        */
        Split4(const AABB& original);
    };
}