#pragma once

#include <Ace/EntityManager.h>
#include <Ace/EntityHandle.h>

namespace ace
{

    class Entity
    {
        EntityManager::EntityHandle* m_handle;

        /**
        @brief Disabled copy- and move-ctor, assignment left as-is. This means that assigning an entity to another means they share entities
        */
        Entity(const Entity&) = delete;
        Entity(Entity&&) = delete;


      //  Entity(Entity&& other) :
      //      m_handle(std::move(other.m_handle))
      //  {
      //
      //  }
    public:


        Entity(EntityManager& manager = EntityManager::DefaultManager()) :
            m_handle(manager.CreateEntity())
        {

        }

        Entity(EntityManager::EntityHandle* handle) :
            m_handle(handle)
        {

        }

        inline static void Clone(Entity& target, const Entity& other)
        {
            target.m_handle->Clone(target.m_handle, other.m_handle);
        }

        template <typename CompType>
        inline bool HasComponent() const
        {
            return m_handle->Has<CompType>();
        }

        inline UInt32 ComponenCount() const
        {
            return m_handle->Count();
        }

        template <typename CompType>
        inline EntityManager::ComponentHandle<CompType>* AddComponent(const CompType& component)
        {
            return m_handle->Add(component);
        }

        template <typename CompType>
        inline void Reserve(const UInt32 size)
        {
            m_handle->Reserve<CompType>(size);
        }

        template <typename CompType>
        inline void RemoveComponent(EntityManager::ComponentBaseHandle* component = nullptr)
        {
            m_handle->Remove<CompType>(component);
        }

        template <typename CompType>
        inline EntityManager::ComponentHandle<CompType>* GetComponent(const UInt32 index = 0u)
        {
            return m_handle->Get<CompType>(index);
        }

        template <typename CompType>
        inline std::vector<EntityManager::ComponentHandle<CompType>*> GetComponents()
        {
            return m_handle->Components<CompType>();
        }

        inline EntityManager::EntityHandle* operator->()
        {
            return m_handle;
        }

        inline void Destroy()
        {
            m_handle->Destroy();
            m_handle = nullptr;
        }

        inline operator bool() const
        {
            return m_handle != nullptr;
        }


    };

}