#include <Ace/Debugger.h>

#if ACE_DEBUG

#include <Ace/Log.h>
#include <Ace/Types.h>
#include <Ace/Transform.h>

namespace ace
{



    void LogDebug(const float f, const char* msg)
    {
        Logger::LogDebug("%s: %f", msg, f);
    }

    void LogDebug(const Matrix2& m, const char* msg)
    {
        Logger::LogDebug("%s:", msg);
        LogDebug(Vector2(m.dataRow(0)), "Row 0");
        LogDebug(Vector2(m.dataRow(1)), "Row 1");
    }
    void LogDebug(const Matrix3& m, const char* msg)
    {
        Logger::LogDebug("%s:", msg);
        LogDebug(Vector3(m.dataRow(0)), "Row 0");
        LogDebug(Vector3(m.dataRow(1)), "Row 1");
        LogDebug(Vector3(m.dataRow(2)), "Row 2");
    }
    void LogDebug(const Matrix4& m, const char* msg)
    {
        Logger::LogDebug("%s:", msg);
        LogDebug(Vector4(m.dataRow(0)), "Row 0");
        LogDebug(Vector4(m.dataRow(1)), "Row 1");
        LogDebug(Vector4(m.dataRow(2)), "Row 2");
        LogDebug(Vector4(m.dataRow(3)), "Row 3");
    }

    void LogDebug(const math::Quaternion& q, const char* msg)
    {
        Logger::LogDebug("%s: Scalar: %f", msg, q.S);
        LogDebug(Vector3(q._v), "Vector");
    }

    void LogDebug(const Transform& t, const char* msg)
    {
        Logger::LogDebug(msg);
        LogDebug(t.GetModel(), "Model");
        LogDebug(t.GetPosition(), "Position");
        LogDebug(t.GetRotation(), "Rotation");
        LogDebug(t.GetScale(), "Scale");
    }

    void LogDebug(const Vector2& v, const char* msg)
    {
        Logger::LogDebug("%s: X: %f, Y: %f", msg, v.x, v.y);
    }
    void LogDebug(const Vector3& v, const char* msg)
    {
        Logger::LogDebug("%s: X: %f, Y: %f, Z: %f", msg, v.x, v.y, v.z);
    }
    void LogDebug(const Vector4& v, const char* msg)
    {
        Logger::LogDebug("%s: X: %f, Y: %f, Z: %f, W: %f", msg, v.x, v.y, v.z, v.w);
    }



}

#endif // ACE_DEBUG
