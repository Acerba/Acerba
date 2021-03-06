#pragma once

#include <Ace/EntityManager.h>

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

        virtual EntityManager::ComponentBaseHandle* Clone(EntityManager::EntityHandle* target, EntityManager::EntityHandle* other) = 0;
        virtual void Delete() = 0;
        virtual const void* Get() const = 0;
        virtual void* Get() = 0;
    };


    template <typename CompType>
    struct EntityManager::ComponentHandle final : public EntityManager::ComponentBaseHandle
    {
    private:
        template <typename Type>
        struct Cloner
        {
            static CompType Clone(void* comp)
            {
                return *static_cast<Type*>(comp);
            }
        };
        template <typename Type>
        struct Cloner<Type*>
        {
            static CompType Clone(void* comp)
            {
				// Memory Copy?
                return static_cast<Type*>(comp);
            }
        };

    public:


        ComponentHandle(EntityManager::EntityHandle* handle, const UInt32 index) :
            ComponentBaseHandle(EntityManager::ComponentID::GetID<CompType>(), handle, index)
        {

        }

        ~ComponentHandle()
        {

        }


        /**
        @brief Clones a component.
        @param[in, out] target Target owner entity of the new component.
        @param[in, out] other Owner entity of the original component.
        @return Returns component pointer owned by 'target'
        */
        EntityManager::ComponentBaseHandle* Clone(EntityManager::EntityHandle* target, EntityManager::EntityHandle* other) override
        {
            EntityManager::ComponentHandle<CompType>* cloned = new EntityManager::ComponentHandle<CompType>(target, EntityManager::ComponentPool<CompType>::GetIndex());
            EntityManager::ComponentPool<CompType>::Push(cloned, Cloner<CompType>::Clone(Get()));
            return cloned;
        }


        /**
        @brief Destroys this component.
        */
        void Delete() override
        {
            ComponentPool<CompType>::Pop(this);
        }


        /**
        @return Retrieves a pointer to component as void*
        */
        const void* Get() const override
        {
           return &ComponentPool<CompType>::GetPool().m_components[index];
        }
        void* Get() override
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }

        /**
        @return Retrieves a reference to the component
        */
        inline const CompType& GetRef() const
        {
            return ComponentPool<CompType>::GetPool().m_components[index];
        }
        inline CompType& GetRef()
        {
            return ComponentPool<CompType>::GetPool().m_components[index];
        }


        /**
        @return Retrieves a pointer to the component.
        */
        inline operator const CompType*() const
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }
        inline operator CompType*()
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }

        /**
        @return Retrieves a pointer to the component.
        */
        inline const CompType* operator->() const
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }
        inline CompType* operator->()
        {
            return &ComponentPool<CompType>::GetPool().m_components[index];
        }


    };

    template <typename CompType>
    using ComponentHandle = EntityManager::ComponentHandle<CompType>;

}