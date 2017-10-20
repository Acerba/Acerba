#pragma once

#include <Ace/CollisionData.h>

#include <Ace/UUID.h>

#include <vector>

namespace ace
{
    struct Collidable;

    struct BVH final
    {

        using CollidableID = UUID<BVH>;

        static void AddCollidable(Collidable& c);

        static void RemoveCollidable(const Collidable& c);

        static void RemoveCollidable(const UInt32 id);

        static void Update();

        static const std::vector<CollisionData>& GetCollisionData();

        BVH() = delete;
        ~BVH() = delete;
        BVH(BVH&&) = delete;
        BVH(const BVH&) = delete;
        BVH& operator=(BVH&&) = delete;
        BVH& operator=(const BVH&) = delete;
    };
}
