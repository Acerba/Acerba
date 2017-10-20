#include <Ace/BVH.h>
#include <Ace/Collidable.h>

#include <Ace/Log.h>

#include <algorithm> // std::find
#include <unordered_map>

namespace ace
{

    struct BVHImpl final
    {
        std::unordered_map<UInt32, std::shared_ptr<Collidable>> collidables;
    } bvh;


    void BVH::AddCollidable(Collidable& c)
    {
        if (!bvh.collidables.insert({c.GetID(), c.GetShared()}).second)
        {
            Logger::LogError("BVH: Collidable already in BVH. ID: %i", c.GetID());
        }
    }
    
    void BVH::RemoveCollidable(const UInt32 id)
    {
        if (bvh.collidables.erase(id) == 0u)
        {
            Logger::LogError("BVH: Collidable not in BVH. ID: %i", id);
        }
    }

    void BVH::RemoveCollidable(const Collidable& c)
    {
        RemoveCollidable(c.GetID());
    }
    
    void BVH::Update()
    {

    }
    
    const std::vector<CollisionData>& BVH::GetCollisionData()
    {
        return {};
    }


}