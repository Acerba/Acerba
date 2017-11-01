#pragma once

#include <Ace/IntTypes.h>
#include <Ace/TypesFwd.h> // Vector2, Matrix2

namespace ace
{
    struct Collidable;
    struct CollidableImpl;
    
    struct BVH final
    {
        /**
            @brief Adds a CollidableImpl to the BVH.
            @param[in] position Position of the collidable.
            @param[in] rotation Rotation of the collidable.
            @warning Do not call yourself. Called by Collidable ctor.
            @see Collidable()
            @return CollidableImpl for the Collidable ctor.
        */
        static CollidableImpl& AddCollidable(const math::Vector2& position, const math::Matrix2& rotation);
        
        /**
            @brief Remove a collidable from the BVH.
            @param[in] c Collidable to remove.
        */
        static void RemoveCollidable(const Collidable& c);
        
        /**
            @brief Remove a collidable from the BVH.
            @param[in] id ID of the CollidableImpl to remove.
        */
        static void RemoveCollidable(const UInt32 id);
        
        /**
            @brief Reserves memory for the collidables to be added to the BVH.
            @param[in] size Number of collidables worth of memory to reserve.
        */
        static void Reserve(const UInt32 size);
        
        /**
            @brief Reset collisions from all collidables.
        */
        static void ResetAllCollisions();

        /**
            @brief Refreshes the bounding volumes in the BVH.
            This makes sure there are as few collisions to check as possible when some form of UpdateCollisions is called.
        */
        static void Update();

        /**
            @brief Calls UpdateCollisions on to all collidables added to the BVH. Calls Update internally.
        */
        static void UpdateAllCollisions();
        
        /**
            @brief Updates collisions regarding c. Make sure you have called BVH::Update before.
            @param[in, out] c Collidable whose collisions to update. Also marks the collisions on to the other collidables, if any.
        */
        static void UpdateCollisions(Collidable& c);
        
        BVH() = delete;
        ~BVH() = delete;
        BVH(BVH&&) = delete;
        BVH(const BVH&) = delete;
        BVH& operator=(BVH&&) = delete;
        BVH& operator=(const BVH&) = delete;
    };
}
