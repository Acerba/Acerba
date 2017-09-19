#pragma once

#include <Ace/EntityManager.h>
#include <Ace/EntityHandle.h>

namespace ace
{

    class Entity
    {
        EntityManager::EntityHandle* m_handle;

        /**
        @brief Disabled copy-ctor, assignment left as-is.
        This means that assigning an entity to another will cause them to share the components present at the moment of assigning.
        */
        Entity(const Entity&) = delete;

    public:

        Entity(Entity&&) = default;
        Entity& operator=(const Entity&) = default;

        /**
        @brief Default constructor.
        @param[in, out] manager Assigns entity to a manager. Uses default manager if not specified.
        @return Returns entity.
        */
        Entity(EntityManager& manager = EntityManager::DefaultManager()) :
            m_handle(manager.CreateEntity())
        {

        }

        /**
        @brief Copy-ctor from EntityHandle
        @param[in, out] handle Pointer of EntityHandle
        @return Returns entity
        */
        Entity(EntityManager::EntityHandle* handle) :
            m_handle(handle)
        {

        }


        /**
        @brief Clones an entity 'other' into entity 'target'. No effect if target has components tied to it.
        @param[in, out] target Target entity of the cloning process. User should use fresh entity here.
        @param[in] other All attributes and components tied to this will be copied to the target and will not be shared between the entities.
        */
        inline static void Clone(Entity& target, const Entity& other)
        {
            if (target.ComponentCount() > 0u)
                return;

            EntityManager::EntityHandle::Clone(target.m_handle, other.m_handle);
        }


        /**
        @brief Check whether entity has a component of type 'CompType'.
        @return Returns true if entity has any components of target type.
        */
        template <typename CompType>
        inline bool HasComponent() const
        {
            return m_handle->Has<CompType>();
        }


        /**
        @brief Returns amount of components tied to this entity.
        @return Number of components tied to this entity.
        */
        inline UInt32 ComponentCount() const
        {
            return m_handle->Count();
        }


        /**
        @brief Marks child as a child of this and this as a parent of child.
        @param[in, out] child Will be child of this.
        */
        void AddChild(Entity& child)
        {
            m_handle->AddChild(child.m_handle);
        }


        /**
            @brief Retrieves the number of children this entity has.
        */
        UInt32 ChildCount() const
        {
            return m_handle->ChildCount();
        }


        /**
            @brief Retrieves the number of children and their children have recursively.
        */
        UInt32 ChildCountTotal() const
        {
            return m_handle->ChildCountTotal();
        }


        /**
            @brief Retrieves the index'th child of this
            @param[in] index Index of the child. Defaults to first child.
            @see ChildCount
            @return Entity built from child. Internally same as actual child.
            @warning Check for validity with bool() operator after return.
        */
        Entity GetChild(const UInt32 index = 0u)
        {
            //Check for validity with bool() after this
            return Entity(m_handle->GetChild(index));
        }

        /**
        @brief Retrieves constant the index'th child of this
        @param[in] index Index of the child. Defaults to first child.
        @see ChildCount
        @return Entity built from child. Internally same as actual child.
        @warning Check for validity with bool() operator after return.
        */
        const Entity GetChild(const UInt32 index = 0u) const
        {
            //Check for validity with bool() after this
            return Entity(m_handle->GetChild(index));
        }


        /**
        @brief Removes target and all its children.
        @param[in, out] target  Must be valid pointer. Target and all its children will be invalidated on this call.
        */
        static void RemoveChild(Entity& target)
        {
            EntityManager::EntityHandle::RemoveChild(target.m_handle);
        }


        /**
        @brief Adds a component of type 'CompType' to this entity.
        @param[in] component Anything of type 'CompType'. The data is copied upon creation, so param does not need to exist after this.
        @return Pointer to the created Component.
        */
        template <typename CompType>
        inline EntityManager::ComponentHandle<CompType>* AddComponent(const CompType& component)
        {
            return m_handle->Add(component);
        }


        /**
        @brief Reserve 'size' amount of elements for components of type 'CompType'. Note that this is a Type-wise reservation.
        @param[in] size Target size to reserve to. No effect if current reserved size is same or larger.
        */
        template <typename CompType>
        inline void Reserve(const UInt32 size)
        {
            m_handle->Reserve<CompType>(size);
        }



        /**
        @brief Removes a component of 'CompType', pointed to by 'component'. Removes first component if none specified. No effect if no components of correct type present.
        @param[in, out] component Pointer to component to remove. Invalid after this function.
        */
        template <typename CompType>
        inline void RemoveComponent(EntityManager::ComponentHandle<CompType>* component = nullptr)
        {
            m_handle->Remove<CompType>(component);
        }

        /**
            @brief Sets this as a child of parent
            @param[in, out] parent New parent of this.
        */
        void SetParent(Entity& parent)
        {
            parent.AddChild(*this);
        }


        /**
        @brief Retrieves index'th component pointer of type 'CompType'. First component by default.
        @param[in] index Index of the component. Valid if smaller than count.
        @see ComponentCount()
        @return Component pointer. Nullptr if index is invalid.
        */
        template <typename CompType>
        inline EntityManager::ComponentHandle<CompType>* GetComponent(const UInt32 index = 0u)
        {
            return m_handle->Get<CompType>(index);
        }

        /**
        @brief Retrieves constant index'th component pointer of type 'CompType'. First component by default.
        @param[in] index Index of the component. Valid if smaller than count.
        @see ComponentCount()
        @return Component pointer. Nullptr if index is invalid.
        */
        template <typename CompType>
        inline const EntityManager::ComponentHandle<CompType>* GetComponent(const UInt32 index = 0u) const
        {
            return m_handle->Get<CompType>(index);
        }


        /**
        @brief Retrieves component pointers of type 'CompType'.
        @warning These pointers are used by the engine, and manually changing them outside of the engine functions can cause memory leaks.
        @return Returns a vector of component pointers. Empty vector if no components present.
        */
        template <typename CompType>
        inline std::vector<EntityManager::ComponentHandle<CompType>*> GetComponents()
        {
            return m_handle->Components<CompType>();
        }


        /**
        @brief Allows access to this entitys handle.
        @return Pointer to the handle.
        */
        inline const EntityManager::EntityHandle* operator->() const
        {
            return m_handle;
        }
        inline EntityManager::EntityHandle* operator->()
        {
            return m_handle;
        }


        /**
        @brief Destroys this entity and all components tied to it.
        */
        inline void Destroy()
        {
            if (m_handle) EntityManager::DestroyEntity(m_handle, *m_handle->manager);
        }


        /**
        @brief Checks whether the entity is valid.
        @return Returns true if the entity is valid.
        */
        inline operator bool() const
        {
            return m_handle != nullptr;
        }


    };

}