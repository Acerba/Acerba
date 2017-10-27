#include <Ace/CollidableImpl.h>

#include <Ace/BVH.h>
#include <Ace/Log.h>

namespace ace
{
    using Matrix2 = CollidableImpl::Matrix2;
    using Vector2 = CollidableImpl::Vector2;

    std::unordered_set<std::shared_ptr<CollidableImpl>> s_collidableImpls;

    CollidableImpl::CollidableImpl(const Vector2& position, const Matrix2& rotation) :
        m_collisions(), m_aabb(), m_rotation(rotation), m_position(position), m_owner(nullptr)
    {
        
    }

    CollidableImpl::~CollidableImpl()
    {
        const UInt32 size = s_collidableImpls.size();
        Logger::LogError("Began destructing CollidableImpl, number of collidables: %i", size);
        Logger::LogError("Ending destructing CollidableImpl");
    }


    CollidableImpl& CollidableImpl::CreateCollidableImpl(const Vector2& position, const Matrix2& rotation)
    {
        auto result = s_collidableImpls.insert(std::make_shared<CollidableImpl>(position, rotation));
        if (!result.second)
        {
            Logger::LogError("Collidable already exists, using old");
        }
        return *result.first->get();
    }

    void CollidableImpl::Destroy()
    {
        try
        {
            const UInt32 removedCount = s_collidableImpls.erase(GetShared());
            Logger::LogError("Removed %i collidables", removedCount);
        }
        catch (std::bad_weak_ptr e)
        {
            Logger::LogError("Caught bad_weak_ptr");
        }
    }


    void CollidableImpl::Reserve(const UInt32 size)
    {
        s_collidableImpls.reserve(size);
    }

    void CollidableImpl::UpdateCollisions()
    {
        BVH::UpdateCollisions(*this);
    }

}
