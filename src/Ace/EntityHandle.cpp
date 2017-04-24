#include <Ace/EntityHandle.h>

namespace ace
{


    void EntityManager::EntityHandle::PushHandle(EntityManager::ComponentBaseHandle* handle)
    {
        if (!m_first && !m_last)
        {
            m_first = m_last = handle;
        }
        else
        {
            m_last->next = handle;
            m_last = handle;
        }
        ++m_componentCount;
    }


    void EntityManager::EntityHandle::PopHandle(EntityManager::ComponentBaseHandle* handle)
    {
        if (m_first == m_last && m_first == handle)
        {
            m_first = m_last = nullptr;
        }
        else if (m_first == handle)
        {
            m_first = handle->next;
        }
        else
        {
            EntityManager::ComponentBaseHandle* prev = m_first;

            while (prev->next != handle && prev)
            {
                prev = prev->next;
            }

            if (handle == m_last)
            {
                prev->next = nullptr;
                m_last = prev;
            }
            else
            {
                prev->next = handle->next;
            }
        }
        --m_componentCount;
    }


    void EntityManager::EntityHandle::AddChild(EntityHandle* target)
    {
        if (!target)
            return;
        auto* root = GetRoot();
        root->RemoveChild(target);
        m_children.emplace_front(target);
    }


    UInt32 EntityManager::EntityHandle::ChildCount() const
    {
        return static_cast<UInt32>(std::distance(m_children.begin(), m_children.end()));
    }


    UInt32 EntityManager::EntityHandle::ChildCountTotal() const
    {
        UInt32 count = ChildCount();
        for (const auto& itr : m_children)
            count += itr->ChildCountTotal();
        return count;
    }


    EntityManager::EntityHandle* EntityManager::EntityHandle::GetChild(UInt32 index)
    {
        auto itr = m_children.begin();
        std::advance(itr, index);
        return (itr == m_children.end() ? nullptr : *itr);
    }


    EntityManager::EntityHandle* EntityManager::EntityHandle::GetRoot()
    {
        EntityHandle* parent = this;
        while (parent->m_parent)
        {
            parent = parent->m_parent;
        }
        return parent;
    }


    void EntityManager::EntityHandle::RemoveChild(EntityHandle* target)
    {
        if (!target)
            return;
        if (!target->m_parent)
            return;

        target->m_parent->m_children.remove_if([&target](EntityHandle* handle){ return handle == target; });
    }


    void EntityManager::EntityHandle::Clone(EntityHandle* target, EntityHandle* other)
    {
        EntityManager::ComponentBaseHandle* otherCurrent = other->m_first;

        while (otherCurrent)
        {
            target->PushHandle(otherCurrent->Clone(target, other));
            otherCurrent = otherCurrent->next;
        }
    }


    void EntityManager::EntityHandle::Destroy()
    {
        EntityManager::ComponentBaseHandle* current = m_first;
        EntityManager::ComponentBaseHandle* temp = nullptr;

        while (current)
        {
            temp = current;
            current = current->next;
            temp->Delete();
        }
    }


    inline UInt32 EntityManager::EntityHandle::Count() const
    {
        return m_componentCount;
    }


    EntityManager::EntityHandle::EntityHandle(EntityManager* manager) :
        transform(),
        manager(manager),
        m_children(),
        m_first(nullptr),
        m_last(nullptr),
        m_next(nullptr),
        m_parent(nullptr),
        m_componentCount(0u)
    {

    }


    EntityManager::EntityHandle::~EntityHandle()
    {

    }

}