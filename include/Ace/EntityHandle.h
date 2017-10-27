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
        bool m_isStatic;


        void PushComponentHandle(EntityManager::ComponentBaseHandle* handle);

        void PopComponentHandle(EntityManager::ComponentBaseHandle* handle);

        ACE_DISABLE_COPY(EntityHandle)

    public:

        /**
            @brief Constructor.
            @param[in, out] manager EntityManager to 
        */
        EntityHandle(EntityManager* manager);

        /**
            @brief Destructor.
        */
        ~EntityHandle();

        /**
            @brief Marks child as a child of this and this as a parent of child.
            @param[in, out] child Will be child of this. Must be valid pointer.
        */
        void AddChild(EntityHandle* child);

        /**
            @brief Adds a component of CompType to the EntityHandle.
            @param[in] component Component to add.
            @return Pointer to the ComponentHandle containing the added component.
        */
        template <typename CompType>
        ComponentHandle<CompType>* AddComponent(const CompType& component)
        {
            ComponentHandle<CompType>* handle = new ComponentHandle<CompType>(this, ComponentPool<CompType>::GetIndex());
            ComponentPool<CompType>::Push(handle, component);
            PushComponentHandle(handle);
            return handle;
        }

        /**
            @brief Get the number of direct children this entity has.
        */
        inline UInt32 ChildCount() const
        {
            return static_cast<UInt32>(std::distance(m_children.begin(), m_children.end()));
        }

        /**
            @brief Get the number of children this entity and the recursive children have.
        */
        UInt32 ChildCountTotal() const;

        /**
            @brief Clones an EntityHandle.
            @param[in] components Should the components be cloned.
            @param[in] children Should the child handles be cloned.
            @param[in, out] manager EntityManager which owns the EntityHandle.
            @return Pointer to the cloned EntityHandle.
        */
        EntityHandle* Clone(const bool children, const bool components, EntityManager& manager = DefaultManager());

        /**
            @return Number of attached components.
        */
        inline UInt32 ComponentCount() const
        {
            return m_componentCount;
        }

        /**
            @brief Destroy all components attached to this EntityHandle.
        */
        void DestroyComponents();


        
        /**
            @brief Retrieves the index'th child of this.
            @param[in] index Index of the child. Defaults to first child.
            @see ChildCount
            @return Pointer to child handle. Nullptr if invalid index.
        */
        EntityHandle* GetChild(const UInt32 index = 0u);

        /**
            @brief Retrieve a pointer to a ComponentHandle of type CompType.
            @param[in] index Index of the component to retrieve.
            @return Pointer to the ComponentHandle.
        */
        template <typename CompType>
        const ComponentHandle<CompType>* GetComponentHandle(UInt32 index = 0u) const
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

        /**
            @brief Retrieve a pointer to a ComponentHandle of type CompType.
            @param[in] index Index of the component to retrieve.
            @return Pointer to the ComponentHandle.
        */
        template <typename CompType>
        ComponentHandle<CompType>* GetComponentHandle(UInt32 index = 0u)
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

        /**
            @brief Retrieves all components of type CompType.
            @return Vector of ComponentHandle pointers.
        */
        template <typename CompType>
        std::vector<ComponentHandle<CompType>*> GetComponents()
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

        /**
            @brief Retrieve pointer to parent EntityHandle.
            @return Nullptr if no parent.
        */
        EntityHandle* GetParent();

        /**
            @brief Retrieves root EntityHandle of this tree
            @return Returns root
        */
        EntityHandle* GetRoot();

        /**
            @brief Checks if the entity has a component of type CompType.
            @return True if component present.
        */
        template <typename CompType>
        bool HasComponent() const
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


        /**
            @brief Queries the entity static status.
            @return True if the entity is static. (Static entities transforms are not recalculated on each frame)
        */
        inline bool IsStatic() const
        {
            return m_isStatic;
        }

        /**
            @brief Remove all children and components recursively.
        */
        void RemoveAllChildren();

        /**
            @brief Removes a direct child, its components, children and their components.
            @param[in, out] child Child and all its children will be invalidated on this call.
        */
        void RemoveChildHandle(EntityHandle* child);

        /**
            @brief Remove a component of CompType from the EntityHandle.
            @param[in, out] handle Handle to the component. Default nullptr. Retrieves it by itself if nullptr.
        */
        template <typename CompType>
        void RemoveComponent(ComponentBaseHandle* handle = nullptr)
        {
            if (!handle)
                handle = GetComponentHandle<CompType>();
            if (handle && handle->entity == this)
            {
                PopComponentHandle(handle);
                ComponentPool<CompType>::Pop(handle);
            }
        }

        /**
            @brief Reserve space for components of type CompType. Has no effect if old size is >= size.
            @param size New size to reserve.
        */
        template <typename CompType>
        static void ReserveComponents(const UInt32 size)
        {
            ComponentPool<CompType>::GetPool().Reserve(size);
        }

        /**
            @brief Set static status of the entity.
            @param[in] isStatic True if the entity should be made static.
        */
        void SetStatic(const bool isStatic)
        {
            m_isStatic = isStatic;
        }

    }; // EntityHandle

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
            if (primaryPool.m_handles[i]->entity->ComponentCount() > 1u && (secondary = entity->GetComponentHandle<SecondaryComponent>()) != nullptr)
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
