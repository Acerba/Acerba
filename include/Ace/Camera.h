#pragma once

#include <Ace/Entity.h>
#include <Ace/Material.h>
#include <Ace/Window.h>

namespace ace
{

    class Camera final
    {
        Entity m_camera;

        Matrix4 m_proj;
        Matrix4 m_view;

        Vector4 m_orthoSize;

    public:

        /**
            @brief Constructor
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
            @return ViewProjection Matrix
        */
        Matrix4 GetMatrix() const;

        /**
            @brief Turns the camera to look at a specific point.
            @param[in] target Target position in world coordinates
            @param[in] up Vector pointing upwards in world coordinates. Defaults to [0,1,0]
        */
        void LookAt(const Vector3& target, const Vector3& up = Vector3(0.f, 1.f, 0.f));

        /**
            @brief Make orthogonal projection.
            @param[in] left Left clipping plane.
            @param[in] right Right clipping plane.
            @param[in] bottom Bottom clipping plane.
            @param[in] top Top clipping plane.
            @param[in] znear Near Z-axis clipping plane.
            @param[in] zfar Far Z-axis clipping plane.
        */
        void MakeOrtho(float left, float right, float bottom, float top, float znear, float zfar);


		/**
			@brief Make orthogonal projection.
			@param[in] horizontal clipping plane.
			@param[in] vertical clipping plane.
			@param[in] znear Near Z-axis clipping plane.
			@param[in] zfar Far Z-axis clipping plane.
		*/
		void MakeOrtho(float horizontal, float vertical, float znear, float zfar);

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


        void Update(const Window& window);
        void Apply(Material& material);

        /**
            @brief Shorthand for retrieving the underlying Entity transform.
            @return Returns Entity transform.
        */
        Transform& GetTransform();
    };
}