#include <Ace/EntityManager.h>
#include <Ace/EntityHandle.h>

namespace ace
{

    std::vector<EntityManager::BasePool*> EntityManager::m_componentPools;

    void EntityManager::RegisterPool(EntityManager::BasePool* poolPtr)
    {
        m_componentPools.emplace_back(poolPtr);
    }

    EntityManager& EntityManager::DefaultManager()
    {
        static EntityManager manager;
        return manager;
    }

    EntityManager::EntityHandle* EntityManager::Entity(EntityManager& manager)
    {
        return manager.CreateEntity();
    }

    void EntityManager::DestroyEntity(EntityManager::EntityHandle* entity, EntityManager& manager)
    {
        if (entity == nullptr || entity->manager != &manager)
            return;

        UInt32 index = 0u;
        while (manager.m_entities[index] != entity)
        {
            ++index;
        }

        manager.m_entities.erase(manager.m_entities.begin() + index);
        entity->Destroy();
        delete entity;
        entity = nullptr;
    }

    EntityManager::EntityManager():
        m_entities()
    {

    }

    EntityManager::~EntityManager()
    {
        for (UInt32 i = 0u; i < m_entities.size(); ++i)
        {
            if (m_entities[i])
            {
                delete m_entities[i];
                m_entities[i] = nullptr;
            }
        }
    }

    EntityManager::EntityHandle* EntityManager::CreateEntity()
    {
        m_entities.emplace_back(new EntityHandle(this));
        return m_entities.back();
    }

	void EntityManager::Update()
	{
		for (UInt32 i = 0; i < m_componentPools.size(); ++i)
		{
			m_componentPools[i]->Update();
		}
	}

}