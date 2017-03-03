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

    public:

        Transform* m_transform;

        /**
        @brief Marks target as a child of this and this as a parent of target.
        @param[in, out] target Will be child of this. Must be valid pointer.
        */
        void AddChild(EntityHandle* target)
        {
            if (!target)
                return;
            GetRoot()->RemoveChild(target);
            target->RemoveChild(target);
            ChildList::AddChild(this, target);
            target->m_parent = this;
        }


        /**
        @brief Get the number of children this entity has
        */
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

        /**
        @brief Retrieves the index'th child of this
        @param[in] index Index of the child. Defaults to first child.
        @see ChildCount
        @return Pointer to child handle. Nullptr if invalid index.
        */
        EntityHandle* GetChild(UInt32 index = 0u)
        {
            if (!m_children.thisChild)
                return nullptr;
            if (index == 0u)
                return m_children.thisChild;

            ChildList* next = m_children.nextChild;
            while (next)
            {
                if (--index == 0u)
                {
                    break;
                }
                next = next->nextChild;
            }

            return next ? next->thisChild : nullptr;
        }

        /**
        @brief Retrieves root entityHandle
        @return Returns root
        */
        EntityHandle* GetRoot()
        {
            EntityHandle* parent = this;
            while (parent->m_parent)
            {
                parent = parent->m_parent;
            }
            return parent;
        }


        /**
        @brief Removes target and all its children.
        @param[in, out] target  Must be valid pointer. Target and all its children will be invalidated on this call.
        */
        static void RemoveChild(EntityHandle* target)
        {
            if (!target)
                return;

            ChildList* prev = &target->m_parent->m_children;

            //Target is first child
            if (prev->thisChild == target)
            {
                *prev = *prev->nextChild;
            }
            //Target is older child
            else
            {
                ChildList* next = prev->nextChild;

                while (next)
                {
                    if (next->thisChild == target)
                    {
                        *prev->nextChild = *next->nextChild;
                        break;
                    }

                    prev = next;
                    next = next->nextChild;
                }
            }
            //Remove target and all its children
            _removeChild(target);
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

	/**
	@brief Executes 'function' for all components of 'PrimaryComponent' type, where they share a common owner entity with 'SecondaryType'.
	@param[in, out] function Function to execute.
	Pointer to the parent entity is passed in as the first argument.
	Pointer to the 'PrimaryComponent' is passed in as the second argument.
	Pointer to the 'SecondaryComponent' is passed in as the third argument.
	*/
	template <typename PrimaryComponent, typename SecondaryComponent, typename Function>
	inline void EntityManager::ForEach(Function function)
	{
		ComponentPool<PrimaryComponent>& primaryPool = ComponentPool<PrimaryComponent>::GetPool();
		ComponentPool<SecondaryComponent>& secondaryPool = ComponentPool<SecondaryComponent>::GetPool();

		ComponentBaseHandle* secondary = nullptr;
		
		EntityHandle* entity = nullptr;
		for (UInt32 i = 0u; i < primaryPool.m_components.size(); ++i)
		{
			entity = primaryPool.m_handles[i]->entity;
			if (primaryPool.m_handles[i]->entity->Count() > 1u && (secondary = entity->Get<SecondaryComponent>()) != nullptr)
			{
				function(primaryPool.m_handles[i]->entity, &primaryPool.m_components[i], &secondaryPool.m_components[secondary->index]);
			}
		}
	}


	/**
	@brief Executes 'function' for all components of 'CompType'.
	@param[in, out] function Function to execute.
	Pointer of the components owner entity is passed in as the first argument to the function.
	Pointer of the component is passed in as the second argument to the function.
	*/
	template <typename CompType, typename Function>
	inline void EntityManager::ForEach(Function function)
	{
		ComponentPool<CompType>& pool = ComponentPool<CompType>::GetPool();
		for (UInt32 i = 0u; i < pool.m_components.size(); ++i)
		{
			function(pool.m_handles[i]->entity, pool.m_components[i]);
		}
	}
}