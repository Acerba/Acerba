#pragma once

#include <Ace/EntityManager.h>

#include <vector>

namespace ace
{

    struct EntityManager::BasePool
    {
        ComponentID componentID;

        virtual const void* Data() const = 0;
        virtual void* Get(const UInt32 index) = 0;
        virtual const void* Get(const UInt32 index) const = 0;
        virtual void Reserve(const UInt32 size) = 0;
        virtual UInt32 Size() const = 0;

    protected:

        BasePool(const ComponentID id) :
            componentID(id)
        {

        }

        virtual ~BasePool()
        {

        }

    private:

        void operator=(const BasePool&) = delete;
    };

    template <typename CompType>
    struct EntityManager::ComponentPool final : public EntityManager::BasePool
    {
    private:

        friend class EntityManager;
        friend struct EntityManager::ComponentHandle<CompType>;

        std::vector<CompType> m_components;
        std::vector<EntityManager::ComponentHandle<CompType>*> m_handles;

    public:

        virtual const void* Data() const
        {
            return m_components.data();
        }
        virtual void* Get(const UInt32 index)
        {
            return &m_components.at(index);
        }
        virtual const void* Get(const UInt32 index) const
        {
            return &m_components.at(index);
        }
        virtual void Reserve(const UInt32 size)
        {
            m_components.reserve(size);
            m_handles.reserve(size);
        }
        virtual UInt32 Size() const
        {
            return m_components.size();
        }


        static ComponentPool& GetPool()
        {
            static ComponentPool pool;
            return pool;
        }

        static UInt32 GetIndex()
        {
            return GetPool().Size();
        }

        static void Push(EntityManager::ComponentHandle<CompType>* handle, const CompType& component)
        {
            GetPool().m_handles.emplace_back(handle);
            GetPool().m_components.emplace_back(component);
        }

        static void Pop(EntityManager::ComponentBaseHandle* handle)
        {
            UInt32 index = handle->index;

            ComponentPool& pool = GetPool();

            Deleter<CompType>::Delete(pool.m_components, index);

            pool.m_handles.erase(pool.m_handles.begin() + index);
            pool.m_components.erase(pool.m_components.begin() + index);

            for (const auto& itr : pool.m_handles)
                if (itr->index > index)
                    --itr->index;

            delete handle;
        }



    private:

        template <typename T>
        struct Deleter
        {
            static void Delete(std::vector<CompType>&)
            {

            }
            static void Delete(std::vector<CompType>&, const UInt32)
            {

            }
        };

        template <typename T>
        struct Deleter<T*>
        {
            static void Delete(std::vector<CompType>& components)
            {
                for (auto& itr : components)
                    if (itr)
                    {
                        delete itr;
                        itr = nullptr;
                    }
            }

            static void Delete(std::vector<CompType>& components, const UInt32 index)
            {
                if (components[index])
                {
                    delete components[index];
                    components[index] = nullptr;
                }
            }
        };

    protected:

        ComponentPool() :
            BasePool(EntityManager::ComponentID(EntityManager::ComponentID::GetID<CompType>())),
            m_components(),
            m_handles()
        {
            RegisterPool(this);
        }

        ~ComponentPool()
        {
            for (auto& itr : m_handles)
                if (itr)
                {
                    delete itr;
                    itr = nullptr;
                }
            Deleter<CompType>::Delete(m_components);
        }

        ACE_DISABLE_COPY(ComponentPool)

    };

}