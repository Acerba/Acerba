#pragma once

#include <Ace/EntityManager.h>
//#include <Ace/ComponentPool.h>

namespace ace
{

    struct EntityManager::ComponentBaseHandle
    {
        EntityManager::ComponentID componentID;
        EntityManager::EntityHandle* const entity;

        UInt32 index;

        EntityManager::ComponentBaseHandle* next;

        ComponentBaseHandle(EntityManager::ComponentID id, EntityManager::EntityHandle* _entity, const UInt32 _index) :
            componentID(id),
            entity(_entity),
            index(_index),
            next(nullptr)
        {

        }

        virtual ~ComponentBaseHandle()
        {

        }

        virtual EntityManager::ComponentBaseHandle* Clone(EntityManager::EntityHandle* other) = 0;
        virtual void Delete() = 0;
        virtual void* Get() = 0;
        virtual const void* Get() const = 0;
    };


    template <typename CompType>
    struct EntityManager::ComponentHandle final : public EntityManager::ComponentBaseHandle
    {
    private:
        template <typename CompType>
        struct Cloner
        {
            static CompType Clone(void* comp)
            {
                return *static_cast<CompType*>(comp);
            }
        };
        template <typename CompType>
        struct Cloner<CompType*>
        {
            static CompType Clone(void* comp)
            {
                return **static_cast<CompType*>(comp);
            }
        };

    public:
        ComponentHandle(EntityManager::EntityHandle* handle, const UInt32 index) :
            ComponentBaseHandle(EntityManager::ComponentID::GetID<CompType>(), handle, index)
        {

        }

        virtual ~ComponentHandle()
        {

        }


        virtual EntityManager::ComponentBaseHandle* Clone(EntityManager::EntityHandle* other)
        {
            EntityManager::ComponentHandle<CompType>* cloned = new EntityManager::ComponentHandle<CompType>(entity, EntityManager::ComponentPool<CompType>::GetIndex());
            EntityManager::ComponentPool<CompType>::Push(cloned, Cloner<CompType>::Clone(Get()));
            return cloned;
        }

        virtual void Delete()
        {
            ComponentPool<CompType>::Pop(this);
        }


        virtual void* Get()
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }
        virtual const void* Get() const
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }


        inline operator CompType*()
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }
        inline operator const CompType*() const
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }


    };


}