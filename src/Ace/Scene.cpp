#include <Ace/Scene.h>

#include <Ace/Entity.h>

namespace ace
{

    Scene::Scene(EntityManager* rootManager) :
        m_root(new Entity(rootManager ? *rootManager : EntityManager::DefaultManager()))
    {

    }

    Scene::~Scene()
    {
        if (m_root)
        {
            delete m_root;
            m_root = nullptr;
        }
    }

    const Entity* Scene::GetRoot() const
    {
        return m_root;
    }
    Entity* Scene::GetRoot()
    {
        return m_root;
    }

}