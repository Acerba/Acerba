#include <Ace/Scene.h>

#include <Ace/Entity.h>
#include <Ace/Matrix4.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/SpriteManager.h>

namespace ace
{

    void ComputeMatrices(Entity& entity, const math::Matrix4& parentModel)
    {
        entity->transform.model =
            (Matrix4::Scale(entity->transform.scale.x, entity->transform.scale.y, entity->transform.scale.z) *
            entity->transform.rotation.ToMatrix4() *
            Matrix4::Translation(entity->transform.position))
            * parentModel;

        const UInt32 count = entity.ChildCount();
        for (UInt32 i = 0u; i < count; ++i)
        {
            Entity child = entity.GetChild(i);
            ComputeMatrices(child, entity->transform.model);
        }
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

    void Scene::Draw(const Camera& camera, const Material* material) const
    {
        SpriteManager::Draw(*this, camera, material);
    }

    void Scene::Update()
    {
        ComputeMatrices(*m_root, math::Matrix4::Identity());
    }

}