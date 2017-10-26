#pragma once

#include <Ace/UUID.h>

namespace ace
{
    struct CollidableImpl;
    
    struct BVH final
    {
        
        using CollidableID = UUID<BVH>;
        
        /**
            @brief Adds a collidable to the BVH.
            @param[in, out] c CollidableImpl to add to the BVH.
        */
        static void AddCollidable(CollidableImpl& c);
        
        /**
            @brief Refreshes the bounding volumes in the BVH.
        */
        static void Refresh();
        
        /**
            @brief Remove a collidable from the BVH.
            @param[in, out] c CollidableImpl to remove.
        */
        static void RemoveCollidable(const CollidableImpl& c);
        
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
            @brief Calls UpdateCollisions on to all collidables added to the BVH. Calls Refresh internally.
         */
        static void UpdateAllCollisions();
        
        /**
            @brief Updates collisions regarding c. Make sure you have called BVH::Refresh before.
            @param[in, out] c CollidableImpl whose collisions to update. Also marks the collisions on to the other collidables, if any.
         */
        static void UpdateCollisions(CollidableImpl& c);
        
        BVH() = delete;
        ~BVH() = delete;
        BVH(BVH&&) = delete;
        BVH(const BVH&) = delete;
        BVH& operator=(BVH&&) = delete;
        BVH& operator=(const BVH&) = delete;
    };
}
