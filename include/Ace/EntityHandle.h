#pragma once

#include <Ace/Component.h>
#include <Ace/ComponentPool.h>
#include <Ace/Transform.h>

#include <forward_list>

namespace ace
{

    struct EntityManager::EntityHandle
    {
        Transform transform;
        EntityManager* manager;

    private:


        std::forward_list<EntityHandle*> m_children;
        EntityManager::ComponentBaseHandle* m_first;
        EntityManager::ComponentBaseHandle* m_last;
        EntityHandle* m_parent;
        UInt32 m_componentCount;


        void PushHandle(EntityManager::ComponentBaseHandle* handle);

        void PopHandle(EntityManager::ComponentBaseHandle* handle);

        ACE_DISABLE_COPY(EntityHandle)

    public:


        /**
            @brief Marks target as a child of this and this as a parent of target.
            @param[in, out] target Will be child of this. Must be valid pointer.
        */
        void AddChild(EntityHandle* target);


        /**
            @brief Get the number of children this entity has.
        */
        UInt32 ChildCount() const;

        /**
            @brief Get the number of children this entity and the recursive children have.
        */
        UInt32 ChildCountTotal() const;

        /**
            @brief Retrieves the index'th child of this.
            @param[in] index Index of the child. Defaults to first child.
            @see ChildCount
            @return Pointer to child handle. Nullptr if invalid index.
        */
        EntityHandle* GetChild(UInt32 index = 0u);

        /**
            @brief Retrieves root entityHandle of this tree
            @return Returns root
        */
        EntityHandle* GetRoot();


        /**
            @brief Removes target and all its children.
            @param[in, out] target  Must be valid pointer. Target and all its children will be invalidated on this call.
        */
        static void RemoveChild(EntityHandle* target);

        EntityHandle(EntityManager* manager);

        ~EntityHandle();

        static void Clone(EntityHandle* target, EntityHandle* other);

        void Destroy();

        UInt32 Count() const;

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
    The 'PrimaryComponent' is passed in as the second argument.
    The 'SecondaryComponent' is passed in as the third argument.
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
                function(primaryPool.m_handles[i]->entity, primaryPool.m_components[i], secondaryPool.m_components[secondary->index]);
            }
        }
    }


    /**
        @brief Executes 'function' for all components of 'CompType'.
        @param[in, out] function Function to execute.
        Pointer of the components owner entity is passed in as the first argument to the function.
        The component is passed in as the second argument to the function.
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