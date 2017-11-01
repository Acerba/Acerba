#include <Ace/CollidableImpl.h>
#include <Ace/Collidable.h>

#include <Ace/BVH.h>
#include <Ace/Log.h>

#include <algorithm>

namespace ace
{
    using Matrix2 = CollidableImpl::Matrix2;
    using Vector2 = CollidableImpl::Vector2;

    CollidableImpl::CollidableImpl(const Vector2& position, const Matrix2& rotation) :
        m_collisions(), m_aabb(), m_rotation(rotation), m_position(position), m_owner(nullptr)
    {
        
    }

    CollidableImpl::~CollidableImpl()
    {

    }


    void CollidableImpl::Destroy()
    {
        BVH::ResetAllCollisions();
        BVH::RemoveCollidable(m_owner->GetID());
    }

    UInt32 CollidableImpl::GetID() const
    {
        return m_owner->GetID();
    }

    bool CollidableImpl::HasCollision(const UInt32 id) const
    {
        return m_collisions.end() != std::find_if(
            m_collisions.begin(), m_collisions.end(),
            [id](const CollidableImpl* const ptr) -> bool
            { return ptr->GetID() == id; }
        );
    }

    void CollidableImpl::SetOwner(Collidable* owner)
    {
        if (owner)
        {
            m_owner = owner;
            m_owner->UpdateAABB();
        }
    }

}
