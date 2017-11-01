#include <Ace/Scene.h>

#include <Ace/Entity.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/Log.h>
#include <Ace/Matrix4.h>
#include <Ace/SpriteManager.h>

namespace ace
{
    void ComputeMatrices(EntityHandle* handle, const math::Matrix4& parentModel, bool hasParentModelChanged)
    {
        if (!handle)
        {
            Logger::Log(Logger::Priority::Warning, "Scene: Update: Skipped null entity");
            return;
        }

        // Changing parent model overrides entity isStatic
        if (hasParentModelChanged || (handle->transform.HasModelChanged() && !handle->IsStatic()))
        {
            handle->transform.SetModel(
                math::MakeScaling(handle->transform.GetScale()) *
                math::ToMatrix<4u>(handle->transform.GetRotation()) *
                math::MakeTranslation(handle->transform.GetPosition()) * parentModel
            );
            hasParentModelChanged = true;
        }

        const UInt32 count = handle->ChildCount();
        for (UInt32 i = 0u; i < count; ++i)
        {
            ComputeMatrices(handle->GetChild(i), handle->transform.GetModel(), hasParentModelChanged);
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
        ComputeMatrices(*m_root, math::s_identity4, false);
    }

}