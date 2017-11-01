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
        m_modelHasChanged(true)
    {

    }

    
    const Matrix4& Transform::GetModel() const
    {
        return m_model;
    }
    Matrix4& Transform::GetModelRef()
    {
        m_modelHasChanged = true;
        return m_model;
    }
    
    const Vector3& Transform::GetPosition() const
    {
        return m_position;
    }
    Vector3& Transform::GetPositionRef()
    {
        m_modelHasChanged = true;
        return m_position;
    }

    const Quaternion& Transform::GetRotation() const
    {
        return m_rotation;
    }
    Quaternion& Transform::GetRotationRef()
    {
        m_modelHasChanged = true;
        return m_rotation;
    }

    const Vector3& Transform::GetScale() const
    {
        return m_scale;
    }
    Vector3& Transform::GetScaleRef()
    {
        m_modelHasChanged = true;
        return m_scale;
    }


    bool Transform::HasModelChanged() const
    {
        return m_modelHasChanged;
    }


    void Transform::SetModel(const Matrix4& model)
    {
        m_modelHasChanged = false;
        m_model = model;
    }

    void Transform::SetModelHasChanged(const bool modelHasChanged)
    {
        m_modelHasChanged = modelHasChanged;
    }

    void Transform::SetPosition(const Vector3& position)
    {
        m_modelHasChanged = true;
        m_position = position;
    }

    void Transform::SetRotation(const Quaternion& rotation)
    {
        m_modelHasChanged = true;
        m_rotation = rotation;
    }

    void Transform::SetScale(const Vector3& scale)
    {
        m_modelHasChanged = true;
        m_scale = scale;
    }
}