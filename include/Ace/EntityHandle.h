#pragma once

#include <Ace/Component.h>
#include <Ace/ComponentPool.h>


namespace ace
{

    class Transform;

    struct EntityManager::EntityHandle
    {
    private:

        struct ChildList
        {
            EntityHandle* thisChild;
            ChildList* nextChild;

            ChildList(EntityHandle* _this) :
                thisChild(_this),
                nextChild(nullptr)
            {

            }

            ~ChildList()
            {
                if (nextChild)
                {
                    delete nextChild;
                    nextChild = nullptr;
                }
            }

            static void AddChild(EntityHandle* parent, EntityHandle* newChild)
            {
                ChildList* next = &parent->m_children;

                if (!next->thisChild)
                {
                    next->thisChild = newChild;
                    return;
                }

                while (next->nextChild)
                {
                    next = next->nextChild;
                }
                next->nextChild = new ChildList(newChild);
            }

        };

        ChildList m_children;
        EntityHandle* m_parent;
        EntityHandle* m_next;
        Transform* m_transform;

    public:


        void AddChild(EntityHandle* childToAdd)
        {
            if (!childToAdd)
                return;
            ChildList::AddChild(this, childToAdd);
            childToAdd->m_parent = this;
        }


        UInt32 ChildCount() const
        {
            UInt32 count = 0u;

            //Recurse children
            ChildList* next = m_children.nextChild;
            if (m_children.thisChild)
                count += 1u + m_children.thisChild->ChildCount();
            while (next)
            {
                if (next->thisChild)
                {
                    count += 1u + next->thisChild->ChildCount();
                    next = next->nextChild;
                }
            }

            //Recurse next
            EntityHandle* nextH = m_next;
            while (nextH)
            {
                count += 1u + nextH->ChildCount();
                nextH = nextH->m_next;
            }

            return count;
        }

        EntityHandle* GetChild(UInt32 index = 0u)
        {
            if (!m_children.thisChild)
                return nullptr;
            if (index == 0u)
                return m_children.thisChild;

            ChildList* next = m_children.nextChild;
            while (next || --index == 0u)
            {
                next = next->nextChild;
            }
            return next->thisChild;
        }


        //Removes a single child from anywhere below this
        static void RemoveChild(EntityHandle* toRemove)
        {
            if (!toRemove)
                return;

            ChildList* prev = &toRemove->m_parent->m_children;
            ChildList* next = nullptr;

            //Target is first child
            if (prev->thisChild == toRemove)
            {
                next = prev->nextChild;
                toRemove->m_parent->m_children.thisChild = next ? next->thisChild : nullptr;
                toRemove->m_parent->m_children.nextChild = next;
            }
            //Target is older child
            else
            {
                next = prev->nextChild;
                while (true)
                {
                    //Failed to remove
                    if (!next)
                    {
                        return;
                    }
                    if (next->thisChild == toRemove)
                    {
                        prev->nextChild->nextChild = next->nextChild ? next->nextChild->nextChild : nullptr;
                        break;
                    }
                    prev = next;
                    next = next->nextChild;
                }
            }
            //Remove target and all its children
            _removeChild(toRemove);
        }

    private:

        //Removes target and all children
        static void _removeChild(EntityHandle* target)
        {
            if (!target)
                return;

            ChildList* nextL = &target->m_children;
            ChildList* tempL = nullptr;

            //Recurse children
            while (nextL)
            {
                tempL = nextL->nextChild;
                _removeChild(nextL->thisChild);
                nextL = tempL;
            }

            EntityHandle* nextH = target->m_next;
            EntityHandle* tempH = nullptr;

            //Recurse next
            while (nextH)
            {
                tempH = nextH->m_next;
                _removeChild(tempH);
                nextH = tempH;
            }
            _removeSelf(target);
        }

        //wont probably work
        static void _removeSelf(EntityHandle* target)
        {
            EntityManager::DestroyEntity(target, *target->manager);
        }


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

        ACE_DISABLE_COPY(EntityHandle)

    public:

        EntityManager* manager;


        EntityHandle(EntityManager* manager) :
            manager(manager),
            m_componentCount(0u),
            m_first(nullptr),
            m_children(nullptr),
            m_last(nullptr),
            m_next(nullptr),
            m_parent(nullptr),
            m_transform(nullptr)
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
                if (current->componentID == ID)
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
        void RemoveComponent(ComponentBaseHandle* handle = nullptr)
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