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

        virtual void Update() = 0;

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
        friend class SpriteManager;

        std::vector<CompType> m_components;
        std::vector<EntityManager::ComponentHandle<CompType>*> m_handles;

        EntityManager::UpdateCallback<CompType> m_update;
        EntityManager::UpdateEntityCallback<CompType> m_entityUpdate;


    public:

        inline void SetUpdateCallback(EntityManager::UpdateCallback<CompType> callback)
        {
            m_update = callback;
        }

        inline void SetUpdateCallback(EntityManager::UpdateEntityCallback<CompType> callback)
        {
            m_entityUpdate = callback;
        }

        const void* Data() const final override
        {
            return m_components.data();
        }

		inline const CompType* DataRef() const
		{
			return m_components.data();
		}

		inline CompType* DataRef()
		{
			return m_components.data();
		}

        void* Get(const UInt32 index) final override
        {
            return &m_components.at(index);
        }

        const void* Get(const UInt32 index) const final override
        {
            return &m_components.at(index);
        }

        void Reserve(const UInt32 size) final override
        {
            m_components.reserve(size);
            m_handles.reserve(size);
        }

        UInt32 Size() const final override
        {
            return m_components.size();
        }

        void Update() final override
        {
            if (m_update)
            {
                for (UInt32 i = 0; i < m_components.size(); ++i)
                {
                    m_update(m_components[i]);
                }
            }

            if (m_entityUpdate)
            {
                for (UInt32 i = 0; i < m_handles.size(); ++i)
                {
                    m_entityUpdate(*(CompType*)m_handles[i]->Get(), m_handles[i]->entity);
                }
            }
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
            m_handles(),
            m_update(nullptr),
            m_entityUpdate(nullptr)
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