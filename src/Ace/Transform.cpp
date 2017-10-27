#include <Ace/Transform.h>

namespace ace
{
    Transform::Transform(
        const Vector3& position,
        const Quaternion& rotation,
        const Vector3& scale
    ) :
        m_model(math::s_identity4),
        m_rotation(rotation),
        m_position(position),
        m_scale(scale),
        m_isModelValid(false)

    {

    }

    
    const Matrix4& Transform::GetModel() const
    {
        return m_model;
    }
    Matrix4& Transform::GetModelRef()
    {
        m_isModelValid = false;
        return m_model;
    }

    bool Transform::GetModelStatus() const
    {
        return m_isModelValid;
    }
    
    const Vector3& Transform::GetPosition() const
    {
        return m_position;
    }
    Vector3& Transform::GetPositionRef()
    {
        m_isModelValid = false;
        return m_position;
    }

    const Quaternion& Transform::GetRotation() const
    {
        return m_rotation;
    }
    Quaternion& Transform::GetRotationRef()
    {
        m_isModelValid = false;
        return m_rotation;
    }

    const Vector3& Transform::GetScale() const
    {
        return m_scale;
    }
    Vector3& Transform::GetScaleRef()
    {
        m_isModelValid = false;
        return m_scale;
    }


    void Transform::SetModel(const Matrix4& model)
    {
        m_isModelValid = true;
        m_model = model;
    }

    void Transform::SetModelStatus(const bool isModelValid)
    {
        m_isModelValid = isModelValid;
    }

    void Transform::SetPosition(const Vector3& position)
    {
        m_isModelValid = false;
        m_position = position;
    }

    void Transform::SetRotation(const Quaternion& rotation)
    {
        m_isModelValid = false;
        m_rotation = rotation;
    }

    void Transform::SetScale(const Vector3& scale)
    {
        m_isModelValid = false;
        m_scale = scale;
    }
}