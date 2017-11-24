#pragma once

#include <Ace/Vector2.h>

namespace ace
{
    struct Collidable;

    struct CollisionData final
    {
        // Other Collidable
        Collidable* other;

        // My closest normal. Center-to-Center if owner is a circle.
        math::Vector2 normal;

        // CollisionData() : other(nullptr), normal(0.f, 0.f) { }

        // Ctor
        CollisionData(Collidable* otherPtr, const math::Vector2& myNormal) :
            other(otherPtr), normal(myNormal)
        {

        }
    };
}