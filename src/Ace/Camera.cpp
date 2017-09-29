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
		// !TEMP! // !TEMP! // !TEMP! // !TEMP! // !TEMP! // !TEMP! //

		// From: A
		// To: E
		// Topic: Scene Matrix calculations
		// Message:	Hey, it seems that scene doesn't update camera's transform for some reason automatically.
		//			So for this case this is handle manually as you can see it. Please fix it when you can.	

		EntityHandle* parent = m_entity->GetParent();

		if (parent != nullptr)
		{
			parent->transform.model =
				(Matrix4::Scale(parent->transform.scale.x, parent->transform.scale.y, parent->transform.scale.z) *
					parent->transform.rotation.ToMatrix4() *
					Matrix4::Translation(parent->transform.position));
		}

		m_entity->transform.model =
			(Matrix4::Scale(m_entity->transform.scale.x, m_entity->transform.scale.y, m_entity->transform.scale.z) *
				m_entity->transform.rotation.ToMatrix4() *
				Matrix4::Translation(m_entity->transform.position)) * (parent != nullptr ? parent->transform.model : Matrix4::Identity());


		// !TEMP! // !TEMP! // !TEMP! // !TEMP! // !TEMP! // !TEMP! //

		Vector3 direction = m_entity->transform.model * m_entity->transform.rotation.ToMatrix4() * Vector4(0, 0, 1, 1);
		Vector3 position = m_entity->transform.model.Transpose() * Vector4(0,0,0,1);

         m_view = Matrix4::LookAt(position, position - Vector3(direction.x, direction.y, direction.z), m_up);
    }

	void Camera::Ortho(const Vector4& size, float aspect)
	{
		m_proj = Matrix4::Ortho(-size.x * aspect, size.x * aspect, -size.y, size.y, size.z, size.w);
	}

	void Camera::LookAt(const Vector3& target)
	{
		m_entity->transform.rotation = Quaternion::LookAt((m_entity->transform.position - target), m_up);
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
		if (windowEvent.type == WindowEventType::Resised || windowEvent.type == WindowEventType::SizeChanged)
		{
			// Aspect ratio.
			m_aspectRatio = windowEvent.data1 / static_cast<float>(windowEvent.data2);
			Ortho(m_ortho, m_aspectRatio);
		}
	}
}
