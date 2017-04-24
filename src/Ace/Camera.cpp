#include <Ace/Camera.h>

namespace ace
{

    Camera::Camera(EntityManager& manager) :
        m_camera(manager)
    {

    }


    void Camera::SetCamera(Entity& parent)
    {
        parent.AddChild(m_camera);
    }

}