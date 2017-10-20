#pragma once

#include <memory>
#include <vector>

namespace ace
{
    struct Collidable;

    struct CollisionData final
    {
        std::shared_ptr<Collidable> owner;
        std::vector<std::shared_ptr<Collidable>> collisions;
    };
}
