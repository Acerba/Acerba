#pragma once

#include <Ace/Entity.h>

namespace ace
{

    class Camera final
    {
        Entity m_entity;
        Matrix4 m_proj;
        Matrix4 m_view;
        Matrix4 m_vp; // v * p
        Vector3 m_ortho; // +-xy, zn, zf
        Vector3 m_up;

        void LookAt();

    public:

        /**
            @brief Constructor.
            @param[in, out] manager EntityManager who handles this entity. Defaults to DefaultManager.
        */
        Camera(EntityManager& manager = EntityManager::DefaultManager());

        /**
            @brief Constructor.
            @param[in] position Position of the camera.
            @param[in] target LookAt target of the camera.
            @param[in] size Clipping planes.
                x: +-horizontal and +- vertical clipping plane.
                y: Near Z-axis clipping plane.
                z: Far Z-axis clipping plane.
                Defaults to [1, 0, 100].
            @param[in] up Vector pointing up in the world. Defaults to [0, 1, 0].
            @param[in, out] manager EntityManager who handles this entity. Defaults to DefaultManager.
        */
        Camera(
            const Vector3& position,
            const Vector3& target,
            const Vector3& size = Vector3(1.f, 0.f, 100.f),
            const Vector3& up = Vector3(0.f, 1.f, 0.f),
            EntityManager& manager = EntityManager::DefaultManager()
        );

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
            @param[in] up Pointer to a up-vector. Defaults to [0, 1, 0].
        */
        void LookAt(const Vector3& target, const Vector3* up = nullptr);

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
            @param[in] horizontal Horizontal clipping plane.
            @param[in] vertical Vertical clipping plane.
            @param[in] znear Near Z-axis clipping plane.
        @param[in] zfar Far Z-axis clipping plane.
        */
        void MakeOrtho(float horizontal, float vertical, float znear, float zfar);
        
        /**
            @brief Make orthogonal projection.
            @param[in] sizes
                x: +-horizontal and +- vertical clipping plane.
                y: Near Z-axis clipping plane.
                z: Far Z-axis clipping plane.
        */
        void MakeOrtho(const Vector3& sizes);

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

        /**
            @brief Set the up-vector of the camera.
            @param[in] up New up-vector.
        */
        void SetUp(const Vector3& up);

        /**
            @brief Update the camera transformations and matrices.
        */
        void Update();

    };
}