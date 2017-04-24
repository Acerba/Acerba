#include <Ace/Scene.h>

#include <Ace/Entity.h>
#include <Ace/Matrix4.h>

namespace ace
{

    void Scene::ComputeMatrices(Entity& entity, const math::Matrix4& parentModel)
    {
        //TODO: No vector3 * matrix3
        //entity->transform.model = (entity->transform.scale * entity->transform.rotation.ToMatrix3() * entity->transform.position) *  parentModel;
        for (UInt32 i = 0u; i < entity.ChildCount(); ++i)
            ComputeMatrices(entity.GetChild(i), entity->transform.model);
    }


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


    const Entity& Scene::GetRoot() const
    {
        return *m_root;
    }


    Entity& Scene::GetRoot()
    {
        return *m_root;
    }


    void Scene::Update()
    {
        ComputeMatrices(*m_root, math::Matrix4::Identity());
    }

}