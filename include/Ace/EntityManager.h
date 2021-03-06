#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Macros.h>

#include <Ace/UUID.h>

#include <vector>

#include <Ace/Material.h>
#include <Ace/Sprite.h>

namespace ace
{

    class EntityManager
    {
        friend class SpriteManager;

    public:

        using ComponentID = UUID<EntityManager>;

        struct EntityHandle;

        template <typename CompType>
        struct ComponentHandle;

        template <typename CompType>
        struct ComponentPool;

        ACE_DISABLE_COPY(EntityManager)

    private:

        struct BasePool;

        struct ComponentBaseHandle;

        static void RegisterPool(BasePool* pool);
        static std::vector<BasePool*> m_componentPools;

        std::vector<EntityHandle*> m_entities;

    public:

        inline static void RegisterAcerbaComponents()
        {
            EntityManager::ComponentID::GetID<Sprite>();
            EntityManager::ComponentID::GetID<Material>();
        }

        static EntityManager& DefaultManager();
        static EntityHandle* Entity(EntityManager& manager = DefaultManager());

        /**
            @brief Update function signature.
        */
        template <typename CompType>
        using UpdateCallback = void(*)(CompType&);

        template <typename CompType>
        using UpdateEntityCallback = void(*)(CompType&, EntityHandle*);

        /**
            @brief Set custom update function for all components of CompType.
            @param[in] callback Function to set as an update function. Gets called when entities are updated. See UpdateCallback
            @return False if no components of the desired type have been created before. True otherwise.
        */
        template <typename CompType>
        bool SetUpdateCallback(UpdateCallback<CompType> callback)
        {
            for (auto itr = m_componentPools.begin(); itr != m_componentPools.end(); ++itr)
            {
                if (ComponentPool<CompType>* pool = dynamic_cast<ComponentPool<CompType>*>(*itr))
                {
                    pool->SetUpdateCallback(callback);
                    return true;
                }
            }
            return false;
        }

        template <typename CompType>
        bool SetUpdateCallback(UpdateEntityCallback<CompType> callback)
        {
            for (auto itr = m_componentPools.begin(); itr != m_componentPools.end(); ++itr)
            {
                if (ComponentPool<CompType>* pool = dynamic_cast<ComponentPool<CompType>*>(*itr))
                {
                    pool->SetUpdateCallback(callback);
                    return true;
                }
            }
            return false;
        }

        /**
            @brief Update all component pools and contained components.
        */
        static void Update();

         
        template <typename CompType>
        static bool SetDefaultUpdateCallback(UpdateCallback<CompType> callback, EntityManager& manager = DefaultManager())
        {
            return manager.SetUpdateCallback(callback);
        }

        template <typename CompType>
        static bool SetDefaultUpdateCallback(UpdateEntityCallback<CompType> callback, EntityManager& manager = DefaultManager())
        {
            return manager.SetUpdateCallback(callback);
        }
 
        /**
        @brief Executes 'function' for all components of 'PrimaryComponent' type, where they share a common owner entity with 'SecondaryType'.
        @param[in, out] function Function to execute.
        Pointer to the parent entity is passed in as the first argument.
        Pointer to the 'PrimaryComponent' is passed in as the second argument.
        Pointer to the 'SecondaryComponent' is passed in as the third argument.
        */
        template <typename PrimaryComponent, typename SecondaryComponent, typename Function>
        static void ForEach(Function function);


        /**
        @brief Executes 'function' for all components of 'CompType'.
        @param[in, out] function Function to execute.
        Pointer of the components owner entity is passed in as the first argument to the function.
        Pointer of the component is passed in as the second argument to the function.
        */
        template <typename CompType, typename Function>
        static void ForEach(Function function);


        /**
        @brief Executes 'function' for all entities managed by 'manager'.
        @param[in, out] function Function to execute for all entities. Entity pointer is passed in as an argument to the function.
        @param[in, out] manager Default manager if not specified.
        */
        template <typename Function>
        inline static void ForEach(Function function, EntityManager& manager = DefaultManager())
        {
            for (UInt32 i = 0u; i < manager.m_entities.size(); ++i)
            {
                function(manager.m_entities[i]);
            }
        }

        /**
        @brief Retrieves all components of type 'CompType' managed by this manager.
        @param[in, out] size Sets the size to the amount of components.
        @return Returns component data.
        */
        template <typename CompType>
        inline static CompType* GetComponents(UInt32& size)
        {
            size = ComponentPool<CompType>::GetIndex();
            return ComponentPool<CompType>::GetPool().DataRef();
        }


        /**
        @brief Retrieves pointer to the index'th entity handled by the 'manager'
        @param[in] index Index of target entity.
        @see EntityCount()
        @param[in, out] manager Manager of the target entity. Default manager if not specified.
        @return Returns pointer to the entity. Nullptr if invalid index.
        */
        inline static EntityHandle* GetEntity(const UInt32 index, EntityManager& manager = DefaultManager())
        {
            return index >= manager.m_entities.size() ? nullptr : manager.m_entities[index];
        }


        /**
        @brief Retrieves amount of entities managed by the 'manager'.
        @param[in, out] manager EntityManager. Default manager if not specified.
        @return Returns amount of entities.
        */
        inline UInt32 EntityCount(const EntityManager& manager = DefaultManager())
        {
            return manager.m_entities.size();
        }


        /**
        @brief Destroys an entity pointed to by 'entity' that is handled by 'manager'.
        @detail Destroys the target entity, all components tied to it and invalidates the pointer.
        No effect if the pointer is invalid or is not managed by 'manager'.
        @param[in, out] entity Pointer to an entity that is managed by 'manager'.
        @param[in, out] manager EntityManager. Default manager if not specified.
        */
        static void DestroyEntity(EntityHandle* entity, EntityManager& manager = DefaultManager());


        /**
        @brief Constructor for users managers.
        */
        EntityManager();
        ~EntityManager();


        /**
        @brief Creates and ties an entity to this manager.
        @return Returns pointer to the created entity.
        */
        EntityHandle* CreateEntity();

    };

    typedef EntityManager::EntityHandle EntityHandle;

}