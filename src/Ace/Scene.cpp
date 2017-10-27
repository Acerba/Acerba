#include <Ace/Scene.h>

#include <Ace/Entity.h>
#include <Ace/Matrix4.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/SpriteManager.h>

#include <Ace/Platform.h>

#if ACE_DEBUG
    #include <Ace/Log.h>
#endif

namespace ace
{

    void ComputeMatrices(Entity& entity, const math::Matrix4& parentModel)
    {
        if (!entity)
        {
            #if ACE_DEBUG
                Logger::LogDebug("ComputeMatrices null entity");
            #endif

            return;
        }

        entity->transform.model =
           math::MakeScaling(entity->transform.scale) *
           math::ToMatrix<4u>(entity->transform.rotation) *
           math::MakeTranslation(entity->transform.position) * parentModel
        ;

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
        ComputeMatrices(*m_root, math::s_identity4);
    }

}