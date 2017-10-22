#include <Ace/BVH.h>
#include <Ace/Collidable.h>

#include <Ace/Log.h>

#include <algorithm> // std::find, std::remove_copy_if
#include <unordered_map>

namespace ace
{
    using Vector2 = Collidable::Vector2;
    
    static const UInt8 limitLeaf = 6u;
    static const UInt8 limitTwo = 12u;
    
    template <typename FItr, typename OItr, typename Pred>
    FItr remove_move_if(FItr first, FItr last, OItr out, Pred p)
    {
        FItr result = first;
        while (first != last)
        {
            if (p(*first))
            {
                *result++ = *first;
            }
            else
            {
                *out++ = *first;
            }
            ++first;
        }
        return result;
    }
    
    class BVHImpl final
    {
        using CollidableContainer = std::unordered_map<UInt32, std::shared_ptr<Collidable>>;
        using IndexContainer = std::vector<UInt32>;
        
        class Node final
        {
            AABB m_aabb;
            std::vector<std::unique_ptr<Node>> m_children;
            IndexContainer m_ids;
            
        public:
            
            Node(const AABB& aabb) : m_aabb(aabb), m_children(), m_ids()
            {
                
            }
            
            Node& AddChild(const AABB& aabb = AABB())
            {
                m_children.emplace_back(std::make_unique<Node>(aabb));
                return *m_children.back().get();
            }
            
            static void UpdateCollisions(
                Collidable& collidable,
                Node& node,
                const CollidableContainer& collidables
            )
            {
                if (AABB::IsColliding(node.m_aabb, collidable.GetAABB()))
                {
                    if (!node.m_children.empty())
                    {
                        for (auto& itr : node.m_children)
                        {
                            UpdateCollisions(
                                collidable,
                                *itr.get(),
                                collidables
                            );
                        }
                    }
                    else
                    {
                        for (const UInt32 id : node.m_ids)
                        {
                            Collidable& other = *collidables.at(id).get();
                            if (Collidable::IsColliding(collidable, other))
                            {
                                collidable.AddCollision(other);
                                other.AddCollision(collidable);
                            }
                        }
                    }
                }
            }
            
            void Update(const AABB& myNewAABB, IndexContainer& myIds);
        }; // Node
        
        CollidableContainer m_collidables;
        Node m_root;
        
        BVHImpl() : m_collidables(), m_root(AABB())
        {
            
        }
        
    public:
        
        static void RemoveMoveIf(IndexContainer& from, IndexContainer& to, const AABB& aabb)
        {
            const CollidableContainer& collidables = BVHImpl::GetBVHImpl().m_collidables;
            (void)remove_move_if(
                from.begin(), from.end(), std::back_inserter(to),
                [&](const UInt32 id) -> bool
                {
                    return !AABB::IsColliding(aabb, collidables.at(id)->GetAABB().GetCenter());
                }
            );
        }
        
        
        void AddCollidable(Collidable& c)
        {
            if (!m_collidables.insert({c.GetID(), c.GetShared()}).second)
            {
                Logger::LogError("BVH: Collidable already in BVH. ID: %i", c.GetID());
            }
        }
        
        static BVHImpl& GetBVHImpl()
        {
            static BVHImpl impl;
            return impl;
        }
        
        CollidableContainer& GetCollidables()
        {
            return m_collidables;
        }
        
        void Rebuild()
        {
            const UInt32 size = m_collidables.size();
            IndexContainer ids;
            ids.reserve(size);
            AABB newRootAABB;
            for (const auto& itr : m_collidables)
            {
                ids.emplace_back(itr.first);
                newRootAABB.Merge(itr.second->GetAABB());
            }
            m_root.Update(newRootAABB, ids);
        }
        
        void RemoveCollidable(const UInt32 id)
        {
            if (m_collidables.erase(id) == 0u)
            {
                Logger::LogError("BVH: Collidable not in BVH. ID: %i", id);
            }
        }
        
        void Reserve(const UInt32 size)
        {
            m_collidables.reserve(size);
        }

        void UpdateAllCollisions()
        {
            Rebuild();
            for (const auto& itr : m_collidables)
            {
                Node::UpdateCollisions(*itr.second.get(), m_root, m_collidables);
            }
        }

        void UpdateCollisions(Collidable& c)
        {
            c.ResetCollisions();
            Node::UpdateCollisions(c, m_root, m_collidables);
        }
        
        
    }; // BVHImpl
    
    
    
    
    
    void BVHImpl::Node::Update(const AABB& myNewAABB, IndexContainer& myIds)
    {
        const UInt32 size = myIds.size();
        m_aabb = myNewAABB;
        m_ids = myIds;
        m_children.clear();
        
        if (size < limitLeaf) // Leaf
        {
            auto& collidables = BVHImpl::GetBVHImpl().GetCollidables();
            for (const UInt32 id : m_ids)
            {
                collidables.at(id)->ResetCollisions();
            }
        }
        else if (size < limitTwo) // Binarytree
        {
            const float splitX = (myNewAABB.min.x + myNewAABB.max.x) * 0.5f;
            const AABB leftAABB(myNewAABB.min, Vector2(splitX, myNewAABB.max.y));
            const AABB rightAABB(Vector2(splitX, myNewAABB.min.y), myNewAABB.max);
            
            IndexContainer leftIds;
            
            BVHImpl::RemoveMoveIf(myIds, leftIds, leftAABB);
            
            AddChild().Update(leftAABB, leftIds);
            AddChild().Update(rightAABB, myIds);
        }
        else // Quadtree
        {
            const AABB::Split split(myNewAABB);
            
            IndexContainer leftTopIds;
            IndexContainer rightTopIds;
            IndexContainer leftBottomIds;
            
            BVHImpl::RemoveMoveIf(myIds, leftTopIds, split.leftTop);
            BVHImpl::RemoveMoveIf(myIds, rightTopIds, split.rightTop);
            BVHImpl::RemoveMoveIf(myIds, leftBottomIds, split.leftBottom);
            
            AddChild().Update(split.leftTop, leftTopIds);
            AddChild().Update(split.rightTop, rightTopIds);
            AddChild().Update(split.leftBottom, leftBottomIds);
            AddChild().Update(split.rightBottom, myIds);
        }
    } // Node::Update
    
    
    
    
    
    void BVH::AddCollidable(Collidable& c)
    {
        BVHImpl::GetBVHImpl().AddCollidable(c);
    }
    
    void BVH::Refresh()
    {
        BVHImpl::GetBVHImpl().Rebuild();
    }
    
    void BVH::RemoveCollidable(const UInt32 id)
    {
        BVHImpl::GetBVHImpl().RemoveCollidable(id);
    }
    
    void BVH::RemoveCollidable(const Collidable& c)
    {
        BVHImpl::GetBVHImpl().RemoveCollidable(c.GetID());
    }
    
    void BVH::Reserve(const UInt32 size)
    {
        BVHImpl::GetBVHImpl().Reserve(size);
    }
    
    void BVH::UpdateAllCollisions()
    {
        BVHImpl::GetBVHImpl().UpdateAllCollisions();
    }
    
    void BVH::UpdateCollisions(Collidable& c)
    {
        BVHImpl::GetBVHImpl().UpdateCollisions(c);
    }
    
}