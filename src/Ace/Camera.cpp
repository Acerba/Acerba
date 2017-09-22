#include <Ace/Camera.h>

namespace ace
{

        // Entity m_entity;
        // Matrix4 m_proj;
        // Matrix4 m_view;
        // Matrix4 m_vp;
        // Vector3 m_up;
        // float m_size;

    Camera::Camera(EntityManager& manager) :
        m_entity(manager),
        m_proj(),
        m_view(),
        m_vp(),
        m_ortho(1.f, 0.f, 100.f),
        m_up(0.f, 1.f, 0.f)
    {
        m_entity->transform.position = Vector3(0.f, 0.f, 1.f);
        MakeOrtho(m_ortho);
        LookAt(Vector3(0));
        Update();
    }

    Camera::Camera(
        const Vector3& position,
        const Vector3& target,
        const Vector3& size,
        const Vector3& up,
        EntityManager& manager
    ) :
        m_entity(manager),
        m_proj(Matrix4::Ortho(-size.x, size.x, -size.x, size.x, size.y, size.z)),
        m_view(Matrix4::LookAt(position, target, up)),
        m_vp(m_view * m_proj),
        m_ortho(size),
        m_up(up)
    {
        m_entity->transform.position = position;
    }

    const Entity& Camera::GetEntity() const
    {
        return m_entity;
    }
    Entity& Camera::GetEntity()
    {
        return m_entity;
    }

    const Matrix4& Camera::GetMatrix() const
    {
        return m_vp;
    }

    const Transform& Camera::GetTransform() const
    {
        return m_entity->transform;
    }
    Transform& Camera::GetTransform()
    {
        return m_entity->transform;
    }

    void Camera::LookAt()
    {
        // TODO:
        // m_view = Matrix4::LookAt(m_entity->transform.position, WHAT, m_up);
    }

    void Camera::LookAt(const Vector3& target, const Vector3* up)
    {
        m_view = Matrix4::LookAt(m_entity->transform.position, target, up ? *up : m_up);
    }


    void Camera::MakeOrtho(float left, float right, float bottom, float top, float znear, float zfar)
    {
        m_proj = Matrix4::Ortho(left, right, bottom, top, znear, zfar);
    }
    void Camera::MakeOrtho(float horizontal, float vertical, float znear, float zfar)
    {
        MakeOrtho(-horizontal, horizontal, -vertical, vertical, znear, zfar);
    }
    void Camera::MakeOrtho(const Vector3& sizes)
    {
        MakeOrtho(-sizes.x, sizes.x, -sizes.x, sizes.x, sizes.y, sizes.z);
    }


    void Camera::Move(const Vector3& distance)
    {
        m_entity->transform.position += distance;
    }


    void Camera::SetPosition(const Vector3& position)
    {
        m_entity->transform.position = position;
    }


    void Camera::SetUp(const Vector3& up)
    {
        m_up = up;
    }


    void Camera::Update()
    {
        LookAt();
        MakeOrtho(m_ortho);
        m_vp = m_view * m_proj;
    }
}
