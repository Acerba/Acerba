#pragma once

#include <Ace/Component.h>
#include <Ace/ComponentPool.h>


namespace ace
{

    class Transform;

    struct EntityManager::EntityHandle
    {
    private:


        /*   struct ChildNode
           {
           EntityHandle* child;
           ChildNode* next;

           ChildNode(EntityHandle* _child = nullptr, ChildNode* _next = nullptr) :
           child(_child),
           next(_next)
           {

           }

           } m_node;*/


        struct ChildList
        {
            ChildList* next;
            EntityHandle* child;
            EntityHandle* parent;
            EntityHandle* const self;


            ChildList(
                EntityHandle * const _self,
                EntityHandle* _parent = nullptr,
                EntityHandle* _child = nullptr,
                ChildList* _next = nullptr
                ) :
                next(_next),
                child(_child),
                parent(_parent),
                self(_self)
            {

            }

            //ACE_DISABLE_COPY(ChildList)

        } m_list;


        EntityHandle* m_parent;
        Transform* m_transform;

        //
        //
        //
        //  e1
        //  |
        //  e2-e4-e5
        //  |     |
        //  e3    e6
        //        |
        //        e7-e11-e12
        //           |    |
        //        e8-e9  e13-e14
        //
        //
        //  e1.AddChild() creates e10
        //  
        //  _removeSingleParent(e6, e11) removes e11, parents e8 and e9 to e6, links e9 to e7
        //
        //  e1
        //  |
        //  e2-e4-e5-e10
        //  |     |
        //  e3    e6
        //        |
        //        e8-e9-e7-e12
        //                  |
        //                 e13-e14


        //  _removSingleParent(e6, e11) without children
        //  
        //        e6
        //        |
        //        e7-e11-e12
        //
        //        e6
        //        |
        //        e7-e12
        //        


        static void _removeSingleParent(EntityHandle* newParent, EntityHandle* oldParent)
        {
            //newparent must be parent of oldparent -> only one level of change is allowed at a time
            if (newParent == oldParent ||
                newParent->m_list.child != oldParent ||
                oldParent->m_list.child == nullptr)
                return;

            //Remove oldParent from the list of nexts under newParent
            ChildList* next = &newParent->m_list.child->m_list;
            while (next && next->next != &oldParent->m_list)
            {
                next = next->next;
            }//next->next is now oldparent, overwrite it
            next->next = oldParent->m_list.next;


            //Reparent all children of oldparent to newParent
            ChildList* child = &oldParent->m_list;
            if (child->child)
            {
                //Newparent will point to first child of oldparent
                newParent->m_list.child = child->child;

                child = &child->child->m_list;
                while (child->next)
                {
                    child->parent = newParent;
                    child = child->next;
                }//now child points to last in next
                child->parent = newParent;
                child->next = oldParent->m_list.next; //Last moved child now points to first on same level
            }
        }


        //Adds child directly below to this
        void AddChild(EntityHandle* childToAdd)
        {
            ChildList* temp = &m_list;

            while (temp->next)
            {
                temp = temp->next;
            }

            temp->next = new ChildList(childToAdd, this);
        }


        // e1
        // |
        // e2-e5
        // |  |
        // e3 e6-e8
        // |  |  |
        // e4 e7 e9

        //

        // e1
        // |
        // e3

        //Remove all children under this
        void RemoveAllChildren()
        {
            //No children to remove
            if (!m_list.child)
                return;
            
            ChildList* child = &m_list.child->m_list;
            ChildList* next = next->next;
            while (child)
            {
                child->self->_removeSelfAndAllChildren();
                child = next;
                if (next)
                    next = next->next;
            }
        }

        //Remove self and all children from below this
        //Remember to call this from one level higher
        void _removeSelfAndAllChildren()
        {
            //Remove all children
            ChildList* self = &m_list;
            if (self->child)
                self->child->_removeSelfAndAllChildren();

            //Remove all nexts
            ChildList* temp = nullptr;
            while (self)
            {
                if (self->next)
                {
                    temp = self->next->next;
                    self->next->self->_removeSelfAndAllChildren();
                }
                self = temp;
            }

            //Remove self
            RemoveSelf(this);
        }

        //wont probably work
        static void RemoveSelf(EntityHandle* handle)
        {
            EntityManager::DestroyEntity(handle, *handle->manager);
        }

        //Removes a single child from anywhere below this
        bool RemoveChild(EntityHandle* childToRemove)
        {
            ChildList* temp = &m_list;

            //I am to be removed
            if (temp->self == childToRemove)
            {
                _removeSingleParent(temp->parent, temp->self);
                return true;
            }


            //If there are children
            if (temp->child)
            {
                if (RemoveChild(childToRemove))
                    return true;
            }

            //No children, go to next
            while (temp || temp->self != childToRemove)
            {
                temp = temp->next;
            }

            //Removal target found
            if (temp)
            {
                _removeSingleParent(temp->parent, temp->self);
                return true;
            }

            return false;
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
            m_first(nullptr),
            m_last(nullptr),
            m_list(this),
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