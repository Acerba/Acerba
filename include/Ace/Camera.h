#pragma once

#include <Ace/Entity.h>
#include <Ace/Material.h>
#include <Ace/Window.h>
#include <Ace/EventManager.h>
#include <Ace/Events.h>

namespace ace
{

    class Camera : public EventBase<WindowEvent>
    {
        Entity m_entity;

        Matrix4 m_proj;
        Matrix4 m_view;
        Matrix4 m_vp; // v * p

        Vector4 m_ortho; // +-xy, zn, zf
        Vector3 m_up;

        void LookAt();
		void Ortho(const Vector4& size, float aspect = 1.0f);

        Vector4 m_orthoSize;
		float m_aspectRatio;

    public:

		static void UpdateMainCamera();

        /**
            @brief Constructor.
            @param[in, out] manager EntityManager who handles this entity. Defaults to DefaultManager.
        */
        Camera(EntityManager& manager = EntityManager::DefaultManager());

        /**
            @brief Retrieve the contained entity.
        */
        const Entity& GetEntity() const;
        /**
            @brief Retrieve the contained entity.
        */
        Entity& GetEntity();

        /**
            @brief Retrieve the view-projection matrix.
            @return Returns view-projection matrix by const reference.
        */
        const Matrix4& GetMatrix() const;

        /**
            @brief Shorthand for retrieving the underlying Entity transform.
            @return Returns Entity transform by const reference.
        */
        const Transform& GetTransform() const;
        /**
            @brief Shorthand for retrieving the underlying Entity transform.
            @return Returns Entity transform by reference.
        */
        Transform& GetTransform();

        /**
            @brief Turn the camera to look at a target.
            @param[in] target New target position to look at.
        */
        void LookAt(const Vector3& target);

        /**
            @brief Make orthogonal projection.
            @param[in] horizontal Horizontal clipping plane.
            @param[in] vertical Vertical clipping plane.
            @param[in] znear Near Z-axis clipping plane.
			@param[in] zfar Far Z-axis clipping plane.
        */
        void Ortho(float horizontal, float vertical, float znear, float zfar);



        /**
            @brief Move the camera.
            @param[in] distance Distance to move the camera.
        */
        void Move(const Vector3& distance);

        /**
            @brief Set the camera to a new location.
            @param[in] position New position for the camera.
        */
        void SetPosition(const Vector3& position);
		Vector3 GetPosition() const;

		Vector4 GetOrtho() const;
		Vector2 GetSize() const;
		float GetAspectRatio() const;

        void Apply();

        /**
            @brief Set the up-vector of the camera.
            @param[in] up New up-vector.
        */
        void SetUp(const Vector3& up);

        /**
            @brief Update the camera transformations and matrices.
        */
        void Update();

		virtual void OnEvent(WindowEvent windowEvent);

    };
}