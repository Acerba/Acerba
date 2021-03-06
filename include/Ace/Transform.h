#pragma once

#include <Ace/Types.h>

namespace ace
{

    class Transform final
    {
        Matrix4 m_model;
        Quaternion m_rotation;
        Vector3 m_position;
        Vector3 m_scale;
        bool m_modelHasChanged;

    public:

        /**
            @brief Transform class, XYZ-coodrinates
            @param[in] position Position, default 0,0,0
            @param[in] rotation Rotation, default 1,0,0,0
            @param[in] scale Scale, default 1,1,1
        */
        Transform(
            const Vector3& position = Vector3(),
            const Quaternion& rotation = Quaternion(),
            const Vector3& scale = Vector3(1.f, 1.f, 1.f)
        );

        /**
            @brief Retrieve model matrix. Does not invalidate model matrix.
            @return Retrieve a constant reference to the model matrix.
        */
        const Matrix4& GetModel() const;
        /**
            @brief Retrieve model matrix. Invalidates model matrix.
            @return Retrieve a reference to the model matrix.
        */
        Matrix4& GetModelRef();

        /**
            @brief Retrieve position. Does not invalidate model matrix.
            @return Retrieve a constant reference to the position.
        */
        const Vector3& GetPosition() const;
        /**
            @brief Retrieve position. Invalidates model matrix.
            @return Retrieve a reference to the position.
        */
        Vector3& GetPositionRef();

        /**
            @brief Retrieve rotation. Does not invalidate model matrix.
            @return Retrieve a constant reference to the rotation.
        */
        const Quaternion& GetRotation() const;
        /**
            @brief Retrieve rotation. Invalidates model matrix.
            @return Retrieve a reference to the rotation.
        */
        Quaternion& GetRotationRef();

        /**
            @brief Retrieve scale. Does not invalidate model matrix.
            @return Retrieve a constant reference to the scale.
        */
        const Vector3& GetScale() const;
        /**
            @brief Retrieve scale. Invalidates model matrix.
            @return Retrieve a reference to the scale.
        */
        Vector3& GetScaleRef();

        /**
            @brief Query model matrix status.
            @return True if the model matrix needs to be recalculated.
        */
        bool HasModelChanged() const;

        /**
            @brief Set the model matrix. Validates model matrix.
        */
        void SetModel(const Matrix4& model);

        /**
            @brief Overwrite the model matrix status.
            @param[in] isModelValid Model matrix status.
            @warning Do not call manually unless you are sure about what you are doing.
        */
        void SetModelHasChanged(const bool isModelValid);

        /**
            @brief Set the position. Invalidates model matrix.
        */
        void SetPosition(const Vector3& position);

        /**
            @brief Set the rotation. Invalidates model matrix.
        */
        void SetRotation(const Quaternion& rotation);

        /**
            @brief Set the scale. Invalidates model matrix.
        */
        void SetScale(const Vector3& scale);
    };
}
