#pragma once

#include <Ace/Entity.h>

namespace ace
{

    class Camera
    {

        Entity m_camera;

    public:

        Camera(EntityManager& manager = EntityManager::DefaultManager());

        void SetCamera(Entity& parent);

    };

}