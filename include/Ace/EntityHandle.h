#pragma once

#include <Ace/Component.h>
#include <Ace/ComponentPool.h>


namespace ace
{

    struct EntityManager::EntityHandle
    {
    private:

        UInt32 m_componentCount;

        EntityManager::ComponentBaseHandle* m_first;
        EntityManager::ComponentBaseHandle* m_last;

        inline void PushHandle(EntityManager::ComponentBaseHandle* handle)
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

        inline void PopHandle(EntityManager::ComponentBaseHandle* handle)
        {
            //TODO: clean this place up

            //WIP not working
            __debugbreak();

            //m_first == handle probably probably not required
            if (m_first == m_last && m_first == handle)
            {
                m_first = m_last = nullptr;
            }
            else
            {
                //TODO: doesn't work properly

                EntityManager::ComponentBaseHandle* prev = m_first;
                if (prev != handle)
                {
                    while (prev->next != handle)
                    {
                        prev = prev->next;
                    }
                }
                handle == m_last ? m_last = prev : prev->next = handle->next;
            }
            --m_componentCount;
        }

        ACE_DISABLE_COPY(EntityHandle)

    public:

        EntityManager* manager;


        EntityHandle(EntityManager* manager) :
            m_first(nullptr),
            m_last(nullptr),
            manager(manager),
            m_componentCount(0u)
        {

        }

        ~EntityHandle()
        {

        }

        inline static void Clone(EntityHandle* target, EntityHandle* other)
        {
            EntityManager::ComponentBaseHandle* otherCurrent = other->m_first;

            while (otherCurrent)
            {
                target->PushHandle(otherCurrent->Clone(target, other));
                otherCurrent = otherCurrent->next;
            }
        }

        // inline static void Clone(EntityHandle* target, EntityHandle* other)
        // {
        //     EntityManager::ComponentBaseHandle* otherCurrent = other->m_first;
        //     EntityManager::ComponentBaseHandle* targetCurrent = target->m_first;
        //
        //     while (otherCurrent)
        //     {
        //
        //         targetCurrent = otherCurrent->Clone(other);
        //         target->PushHandle(targetCurrent);
        //         targetCurrent = targetCurrent->next;
        //         otherCurrent = otherCurrent->next;
        //     }
        // }

        inline void Destroy()
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

        inline UInt32 Count() const
        {
            return m_componentCount;
        }

        template <typename CompType>
        bool Has() const
        {
            const UInt32 ID = EntityManager::ComponentID::GetID<CompType>();
            EntityManager::ComponentBaseHandle* current = m_first;

            while (current)
            {
                if (current->componentID.m_ID == ID)
                {
                    return true;
                }
                current = current->next;
            }
            return false;
        }

        template <typename CompType>
        ComponentHandle<CompType>* Add(const CompType& component)
        {
            ComponentHandle<CompType>* handle = new ComponentHandle<CompType>(this, ComponentPool<CompType>::GetIndex());
            ComponentPool<CompType>::Push(handle, component);
            PushHandle(handle);
            return handle;
        }

        template <typename CompType>
        void Reserve(const UInt32 size)
        {
            ComponentPool<CompType>::GetPool().Reserve(size);
        }

        template <typename CompType>
        void Remove(ComponentBaseHandle* handle = nullptr)
        {
            if (!handle)
                handle = Get<CompType>();
            if (handle && handle->entity == this)
            {
                PopHandle(handle);
                ComponentPool<CompType>::Pop(handle);
            }
        }

        template <typename CompType>
        ComponentHandle<CompType>* Get(UInt32 index = 0u)
        {
            const UInt32 ID = EntityManager::ComponentID::GetID<CompType>();
            EntityManager::ComponentBaseHandle* current = m_first;

            while (current)
            {
                if (current->componentID == ID && index-- == 0u)
                    return static_cast<ComponentHandle<CompType>*>(current);

                current = current->next;
            }
            return nullptr;
        }

        template <typename CompType>
        std::vector<ComponentHandle<CompType>*> Components()
        {
            const UInt32 ID = EntityManager::ComponentID::GetID<CompType>();
            EntityManager::ComponentBaseHandle* current = m_first;

            std::vector<ComponentHandle<CompType>*> components;

            while (current)
            {
                if (current->componentID.GetID<CompType>() == ID)
                    components.emplace_back(static_cast<ComponentHandle<CompType>*>(current));

                current = current->next;
            }
            return components;
        }
    };
}