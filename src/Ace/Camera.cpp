#include <Ace/Camera.h>

namespace ace
{

    Camera::Camera(EntityManager& manager) :
        m_camera(manager)
    {
		m_camera->transform.position = Vector3(0, 0, 1);
        //Move(1);
		MakeOrtho(1, 1, 0, 100);
		LookAt(Vector3(0));
    }

    const Entity& Camera::GetEntity() const
    {
        return m_camera;
    }


    Entity& Camera::GetEntity()
    {
        return m_camera;
    }

    Matrix4 Camera::GetMatrix() const
    {
		return m_view * m_proj;
    }

    void Camera::LookAt(const Vector3& target, const Vector3& up)
    {
		m_view = Matrix4::LookAt(m_camera->transform.position, target, up);
    }

    void Camera::MakeOrtho(float left, float right, float bottom, float top, float znear, float zfar)
    {
        m_proj = Matrix4::Ortho(left, right, bottom, top, znear, zfar);
    }

	void Camera::MakeOrtho(float horizontal, float vertical, float znear, float zfar)
	{
		m_proj = Matrix4::Ortho(-horizontal, horizontal, -vertical, vertical, znear, zfar);
       
        m_orthoSize.x = horizontal; 
        m_orthoSize.y = vertical;
        m_orthoSize.z = znear;
        m_orthoSize.w = zfar;
	}

    void Camera::Move(const Vector3& v)
    {
        m_camera->transform.position += v;
    }

    void Camera::SetPosition(const Vector3& v)
    {
        m_camera->transform.position = v;
    }

    Transform& Camera::GetTransform()
    {
        return m_camera->transform;
    }

    void Camera::Apply()
    {
        Material::Uniform("VP", GetMatrix());
    }

    void Camera::Update(const Window& window)
    {
        float ratio = window.GetSize().x / window.GetSize().y;
        m_proj = Matrix4::Ortho(-m_orthoSize.x * ratio, m_orthoSize.x * ratio, -m_orthoSize.y, m_orthoSize.y, m_orthoSize.z, m_orthoSize.w);

    }

}