#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Macros.h>

#include <vector>

namespace ace
{

    class EntityManager
    {
    public:

        class ComponentID
        {
            const UInt32 m_ID;

            inline static UInt32 GenerateID()
            {
                static UInt32 _id = 0u;
                return ++_id;
            }

            template <typename T>
            struct ID_Handle
            {
                inline static ID_Handle& GetInstance()
                {
                    static ID_Handle handle(GenerateID());
                    return handle;
                }

                inline static UInt32 GetID()
                {
                    return GetInstance().m_ID;
                }

            private:

                UInt32 m_ID;

                ID_Handle(const UInt32 id) :
                    m_ID(id)
                {

                }
            };

        public:

            template <typename T>
            inline static UInt32 GetID(const T&)
            {
                return ID_Handle<T>::GetID();
            }
            template <typename T>
            inline static UInt32 GetID(const T*)
            {
                return ID_Handle<T>::GetID();
            }
            template <typename T>
            inline static UInt32 GetID()
            {
                return ID_Handle<T>::GetID();
            }

            template <typename T>
            ComponentID(const T& value) :
                m_ID(GetID(value))
            {

            }

            ComponentID(const UInt32 id) :
                m_ID(id)
            {

            }

            inline operator UInt32() const
            {
                return m_ID;
            }
        };

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

        static EntityManager& DefaultManager();
        static EntityHandle* Entity(EntityManager& manager = DefaultManager());

        //foreach' && other utilities

        template <typename PrimaryComponent, typename SecondaryComponent, typename Function>
        inline static void ForEach(Function function)
        {
            ComponentPool<PrimaryComponent>& primaryPool = ComponentPool<PrimaryComponent>::GetPool();
            ComponentPool<SecondaryComponent>& secondaryPool = ComponentPool<SecondaryComponent>::GetPool();

            ComponentBaseHandle* secondary = nullptr;

            for (size_t i = 0u; i < primaryPool.m_components.size(); ++i)
            {
                if (primaryPool.m_handles[i]->entity->Count() > 2 && (secondary = primaryPool.m_handles[i]->entity->Get<SecondaryComponent>()) != nullptr)
                {
                    function(primaryPool.m_handles[i]->entity, primaryPool.m_components[i], secondaryPool.m_components[secondary->index]);
                }
            }
        }

        template <typename CompType>
        inline static CompType* GetComponents(UInt32& size)
        {
            size = ComponentPool<CompType>::GetIndex();
            return ComponentPool<CompType>::GetPool().components.data();
        }

        //anything bad in const manager?
        inline static EntityHandle* GetEntity(const UInt32 index, const EntityManager& manager = DefaultManager())
        {
            return index >= manager.m_entities.size() ? nullptr : manager.m_entities[index];
        }


        inline UInt32 EntityCount(const EntityManager& manager = DefaultManager())
        {
            return manager.m_entities.size();
        }

        static void DestroyEntity(EntityHandle* entity, EntityManager& manager = DefaultManager());

        EntityManager();
        ~EntityManager();

        EntityHandle* CreateEntity();

    };



}