#include <Ace/BVH.h>
#include <Ace/Collidable.h>
#include <Ace/CollidableImpl.h>
#include <Ace/Log.h>

#include <algorithm> // std::find_if, std::remove_copy_if
#include <memory> // std::unique_ptr
#include <vector> // std::vector

namespace ace
{
    using Matrix2 = math::Matrix2;
    using Vector2 = math::Vector2;
    
    static const UInt8 limitLeaf = 6u;
    static const UInt8 limitTwo = 12u;
    static const UInt8 limitDepth = 9u;
    
    // Moves all elements from [first, last) to [out], which return true for p.
    template <typename FItr, typename OItr, typename Pred>
    FItr remove_move_if(FItr first, FItr last, OItr out, Pred p)
    {
        FItr result = first;
        while (first != last)
        {
            if (p(*first))
            {
                *out++ = *first++;
            }
            else
            {
                *result++ = *first++;
            }
        }
        return result;
    }
    
    class BVHImpl final
    {
        using IndexContainer = std::vector<UInt32>;

        struct CollidableImpls final
        {
            using Container = std::vector<std::unique_ptr<CollidableImpl>>;
            using Iterator = Container::iterator;
    
            const Container& Data() const
            {
                return m_collidables;
            }
            Container& Data()
            {
                return m_collidables;
            }
    
            CollidableImpl* Find(const UInt32 id)
            {
                Iterator itr = FindItr(id);
                return IsIteratorValid(itr) ? itr->get() : nullptr;
            }
            CollidableImpl* Find(Collidable& c)
            {
                return Find(c.GetID());
            }
            Iterator FindItr(const UInt32 id)
            {
                return std::find_if(
                    m_collidables.begin(), m_collidables.end(),
                    [id](const std::unique_ptr<CollidableImpl>& ptr) -> bool
                    { return ptr->GetID() == id; }
                );
            }
    
            inline bool IsIteratorValid(const Iterator& itr)
            {
                return itr != m_collidables.end();
            }
    
        private:
            Container m_collidables;
        }; // CollidableImpls
        

        class Node final
        {
            AABB m_aabb;
            std::vector<Node> m_children;
            IndexContainer m_ids;
            
        public:
            
            Node(const AABB& aabb) : m_aabb(aabb), m_children(), m_ids()
            {
                
            }
            
            Node& AddChild(const AABB& aabb = AABB())
            {
                m_children.emplace_back(aabb);
                return m_children.back();
            }
            
            static void UpdateCollisions(
                CollidableImpl& collidable,
                Node& node,
                CollidableImpls& collidables
            )
            {
                if (AABB::IsColliding(node.m_aabb, collidable.GetAABB()))
                {
                    if (!node.m_children.empty())
                    {
                        for (auto& itr : node.m_children)
                        {
                            UpdateCollisions(collidable, itr, collidables);
                        }
                    }
                    else
                    {
                        for (const UInt32 id : node.m_ids)
                        {
                            if (id != collidable.GetID())
                            {
                                CollidableImpl* other = collidables.Find(id);
                                if (other && Collidable::IsColliding(collidable.GetOwner(), other->GetOwner()) && !collidable.HasCollision(id))
                                {
                                    collidable.AddCollision(other);
                                    other->AddCollision(&collidable);
                                }
                            }
                        }
                    }
                }
            }
            
            void Update(const AABB& myNewAABB, IndexContainer& myIds, const UInt8 depth);
        }; // Node
        
        CollidableImpls m_collidables;
        Node m_root;
        
        BVHImpl() : m_collidables(), m_root(AABB())
        {
            
        }
        
    public:

        CollidableImpl& AddCollidable(const Vector2& position, const Matrix2& rotation)
        {
            CollidableImpls::Container& data = m_collidables.Data();
            data.emplace_back(std::make_unique<CollidableImpl>(position, rotation));
            return *data.back().get();
        }

        
        static BVHImpl& GetBVHImpl()
        {
            static BVHImpl impl;
            return impl;
        }
        
        void Rebuild()
        {
            CollidableImpls::Container& data = m_collidables.Data();
            const UInt32 size = data.size();
            IndexContainer ids(size);
            AABB newRootAABB;
            for (UInt32 i = 0u; i < size; ++i)
            {
                ids[i] = data[i]->GetID();
                data[i]->GetOwner().UpdateAABB();
                newRootAABB.Merge(data[i]->GetAABB());
            }
            m_root.Update(newRootAABB, ids, 0u);
            ResetAllCollisions();
        }
        
        void RemoveCollidable(const UInt32 id)
        {
            CollidableImpls::Iterator itr = m_collidables.FindItr(id);
            if (m_collidables.IsIteratorValid(itr))
            {
                m_collidables.Data().erase(itr);
            }
            else
            {
                Logger::LogError("RemoveCollidable: Collidable not found. ID: %i", id);
            }
        }

        void RemoveMoveIf(IndexContainer& from, IndexContainer& to, const AABB& aabb)
        {
            (void)remove_move_if(
                from.begin(), from.end(), std::back_inserter(to),
                [&](const UInt32 id) -> bool
                {
                    CollidableImpl* ptr = m_collidables.Find(id);
                    return ptr ? AABB::IsColliding(aabb, ptr->GetAABB().GetCenter()) : false;
                }
            );
        }
        
        void Reserve(const UInt32 size)
        {
            m_collidables.Data().reserve(size);
        }

        void ResetAllCollisions()
        {
            for (const auto& itr : m_collidables.Data())
            {
                itr->ResetCollisions();
            }
        }

        void UpdateAllCollisions()
        {
            Rebuild();
            for (const auto& itr : m_collidables.Data())
            {
                Node::UpdateCollisions(*itr.get(), m_root, m_collidables);
            }
        }

        void UpdateCollisions(Collidable& c)
        {
            if (CollidableImpl* found = m_collidables.Find(c))
            {
                found->ResetCollisions();
                Node::UpdateCollisions(*found, m_root, m_collidables);
            }
            else
            {
                Logger::LogError("UpdateCollisions: Collidable not found, ID: %i", c.GetID());
            }
        }
        
        
    }; // BVHImpl
    
    
    
    
    
    void BVHImpl::Node::Update(const AABB& myNewAABB, IndexContainer& myIds, const UInt8 depth)
    {
        const UInt32 size = myIds.size();
        m_aabb = myNewAABB;
        m_ids = myIds;
        m_children.clear();
        
        if (size < limitLeaf || depth == limitDepth) // Leaf
        {
            // Collisions reset after this call
            // Empty on purpose
        }
        else if (size < limitTwo) // Binarytree
        {
            const AABB::Split2 split(myNewAABB);
            
            IndexContainer leftIds;
            
            BVHImpl::GetBVHImpl().RemoveMoveIf(myIds, leftIds, split.left);
            
            AddChild().Update(split.left, leftIds, depth + 1u);
            AddChild().Update(split.right, myIds, depth + 1u);
        }
        else // Quadtree
        {
            const AABB::Split4 split(myNewAABB);
            
            IndexContainer leftTopIds;
            IndexContainer rightTopIds;
            IndexContainer leftBottomIds;
            
            BVHImpl& impl = BVHImpl::GetBVHImpl();
            impl.RemoveMoveIf(myIds, leftTopIds, split.leftTop);
            impl.RemoveMoveIf(myIds, rightTopIds, split.rightTop);
            impl.RemoveMoveIf(myIds, leftBottomIds, split.leftBottom);

            AddChild().Update(split.leftTop, leftTopIds, depth + 1u);
            AddChild().Update(split.rightTop, rightTopIds, depth + 1u);
            AddChild().Update(split.leftBottom, leftBottomIds, depth + 1u);
            AddChild().Update(split.rightBottom, myIds, depth + 1u);
        }
    } // Node::Update
    
    
    
    
    
    CollidableImpl& BVH::AddCollidable(const Vector2& position, const Matrix2& rotation)
    {
        return BVHImpl::GetBVHImpl().AddCollidable(position, rotation);
    }
    
    void BVH::RemoveCollidable(const UInt32 id)
    {
        BVHImpl::GetBVHImpl().RemoveCollidable(id);
    }
    
    void BVH::RemoveCollidable(const Collidable& c)
    {
        RemoveCollidable(c.GetID());
    }
    
    void BVH::Reserve(const UInt32 size)
    {
        BVHImpl::GetBVHImpl().Reserve(size);
    }

    void BVH::ResetAllCollisions()
    {
        BVHImpl::GetBVHImpl().ResetAllCollisions();
    }

    void BVH::Update()
    {
        BVHImpl::GetBVHImpl().Rebuild();
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