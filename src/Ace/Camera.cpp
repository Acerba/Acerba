#include <Ace/Camera.h>

namespace ace
{
	inline Camera* GetCamera(Camera* camera = nullptr)
	{
		static Camera* s_camera = nullptr;

		if (camera != nullptr)
		{
			s_camera = camera;
		}

		return s_camera;
	}

	void Camera::UpdateMainCamera()
	{
		Camera* camera = GetCamera();

		if (camera != nullptr)
		{
			camera->Update();
		}
	}


	Camera::Camera(EntityManager& manager) :
        m_entity(manager),
        m_proj(),
        m_view(),
        m_vp(),
        m_ortho(1.f, 1.f, 0.0f, 100.f),
        m_up(0.f, 1.f, 0.f),
		m_aspectRatio(1.0f)
    {
        m_entity->transform.position = Vector3(0.f, 0.f, 1.f);
        Ortho(m_ortho);
        LookAt(Vector3(0.0f, 0.0f, 0.0f));

		if (GetCamera() == nullptr)
		{
			Apply();
		}
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
        const Vector3 position(mv::ResizeVektor<3u>(mv::ToVektor(
            mv::Transpose(m_entity->transform.model) * Vector4(0.f,0.f,0.f,1.f)
        )));
        const Vector3 direction(mv::ResizeVektor<3u>(mv::ToVektor(
            m_entity->transform.model *
            mv::ToMatrix<4u>(m_entity->transform.rotation) *
            Vector4(0.f,0.f,-1.f,1.f)
        )));
		m_view = mv::LookAt(position, position - direction, m_up);
    }

	void Camera::Ortho(const Vector4& size, float aspect)
	{
		m_proj = mv::MakeOrthographic(
			-size.x * aspect, size.x * aspect,
			size.y, -size.y,
			size.z, size.w
		);
	}

	void Camera::LookAt(const Vector3& target)
	{
		m_entity->transform.rotation = mv::LookAt2(m_entity->transform.position - target, m_up);
		LookAt();
	}

    void Camera::Ortho(float horizontal, float vertical, float znear, float zfar)
    {
		m_ortho.x = horizontal;
		m_ortho.y = vertical;
		m_ortho.z = znear;
		m_ortho.w = zfar;

		Ortho(m_ortho, m_aspectRatio);
    }

	void Camera::SetUp(const Vector3& up)
	{
		m_up = up;
	}
	
    void Camera::Move(const Vector3& distance)
    {
        m_entity->transform.position += distance;
    }

    void Camera::SetPosition(const Vector3& position)
    {
        m_entity->transform.position = position;
    }

	Vector3 Camera::GetPosition() const
	{
		return m_entity->transform.position;
	}

	Vector4 Camera::GetOrtho() const
	{
		return m_ortho;
	}

	Vector2 Camera::GetSize() const
	{
		return Vector2(m_ortho.x, m_ortho.y);
	}

	float Camera::GetAspectRatio() const
	{
		return m_aspectRatio;
	}

    void Camera::Update()
    {
		if (GetCamera() == this)
		{
			LookAt();
			m_vp = m_view * m_proj;
			Material::Uniform("VP", m_vp);
		}
    }

	void Camera::Apply()
	{
		Window* window = Window::GetCurrent();

		if (window != nullptr)
		{
			m_aspectRatio = window->GetSize().x / static_cast<float>(window->GetSize().y);
		}

		Ortho(m_ortho, m_aspectRatio);

		GetCamera(this);
		Update();
	}

	void Camera::OnEvent(WindowEvent windowEvent)
	{
		if (windowEvent.type == WindowEventType::Resized || windowEvent.type == WindowEventType::SizeChanged)
		{
			// Aspect ratio.
			m_aspectRatio = static_cast<float>(windowEvent.data1) / static_cast<float>(windowEvent.data2);
			Ortho(m_ortho, m_aspectRatio);
		}
	}
}
