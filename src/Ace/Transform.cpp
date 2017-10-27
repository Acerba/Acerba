#include <Ace/Transform.h>

namespace ace
{
    Transform::Transform(
        const Vector3& position,
        const Quaternion& rotation,
        const Vector3& scale
    ) :
        model(math::s_identity4),
        rotation(rotation),
        position(position),
        scale(scale)
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
        m_isModelValid = false;
        m_model = model;
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