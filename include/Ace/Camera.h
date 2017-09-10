#pragma once

#include <Ace/Entity.h>

namespace ace
{

    class Camera
    {
        Entity m_camera;

        Matrix4 m_proj;
        Matrix4 m_view;

    public:

        Camera(EntityManager& manager = EntityManager::DefaultManager());

        const Entity& GetCamera() const;

        Entity& GetCamera();

		/**
			@return ViewProjection Matrix
		*/
        Matrix4 GetMatrix() const;

        void LookAt(const Vector3& target, const Vector3& up = Vector3(0.f, 1.f, 0.f));
        void MakeOrtho(float left, float right, float bottom, float top, float znear, float zfar);

        void Move(const float z);
        void Move(const Vector3& v);

        void SetParent(Entity& parent);
        void SetPosition(const Vector3& v);

		Transform& GetTransform();
	};
}