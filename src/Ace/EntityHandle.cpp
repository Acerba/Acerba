#include <Ace/EntityHandle.h>

#include <algorithm> // std::find

namespace ace
{
    void EntityManager::EntityHandle::PushComponentHandle(EntityManager::ComponentBaseHandle* handle)
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


    void EntityManager::EntityHandle::PopComponentHandle(EntityManager::ComponentBaseHandle* handle)
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


    EntityManager::EntityHandle::EntityHandle(EntityManager* manager) :
        transform(),
        manager(manager),
        m_children(),
        m_first(nullptr),
        m_last(nullptr),
        m_parent(nullptr),
        m_componentCount(0u)
    {

    }

    EntityManager::EntityHandle::~EntityHandle()
    {
        // TODO: fix segfault
        //this->RemoveAllChildren();
    }


    void EntityManager::EntityHandle::AddChild(EntityHandle* child)
    {
        if (!child) return;
        if (child->m_parent)
        {
            child->m_parent->m_children.remove(child);
        }
        child->m_parent = this;
        m_children.emplace_front(child);
    }


    UInt32 EntityManager::EntityHandle::ChildCountTotal() const
    {
        UInt32 count = ChildCount();
        for (const auto& itr : m_children)
            count += itr->ChildCountTotal();
        return count;
    }

    EntityHandle* EntityManager::EntityHandle::Clone(const bool children, const bool components, EntityManager& manager)
    {
        EntityHandle* clone = manager.CreateEntity();

        if (children)
        {
            for (auto& child : m_children)
            {
                clone->AddChild(child->Clone(children, components));
            }
        }

        if (components)
        {
            EntityManager::ComponentBaseHandle* current = m_first;
            while (current)
            {
                clone->PushComponentHandle(current->Clone(clone, this));
                current = current->next;
            }
        }
        return clone;
    }

    void EntityManager::EntityHandle::DestroyComponents()
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

    EntityManager::EntityHandle* EntityManager::EntityHandle::GetChild(const UInt32 index)
    {
        if (index < ChildCount())
        {
            auto itr = m_children.begin();
            std::advance(itr, index);
            return *itr;
        }
        return nullptr;
    }

	EntityManager::EntityHandle* EntityManager::EntityHandle::GetParent()
	{
		return m_parent;
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


    void EntityManager::EntityHandle::RemoveChildHandle(EntityHandle* child)
    {
        if (!child) return;
        auto itr = std::find(m_children.begin(), m_children.end(), child);
        if (itr == m_children.end()) return;
        (*itr)->RemoveAllChildren();
        m_children.remove(child); // Does NOT call dtor of child
        child = nullptr;
    }

    void EntityManager::EntityHandle::RemoveAllChildren()
    {
        this->DestroyComponents();
        for (auto& child : this->m_children)
            this->RemoveChildHandle(child);
        m_children.clear();
    }

}