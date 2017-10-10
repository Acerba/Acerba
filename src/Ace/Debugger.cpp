#include <Ace/Debugger.h>

#if ACE_DEBUG

#include <Ace/Log.h>
#include <Ace/Types.h>
#include <Ace/Transform.h>

namespace ace
{





    void LogDebug(const Matrix2& m, const char* msg)
    {
        Logger::LogDebug("%s:", msg);
        LogDebug(m.rows[0], "Row 0");
        LogDebug(m.rows[1], "Row 1");
    }
    void LogDebug(const Matrix3& m, const char* msg)
    {
        Logger::LogDebug("%s:", msg);
        LogDebug(m.rows[0], "Row 0");
        LogDebug(m.rows[1], "Row 1");
        LogDebug(m.rows[2], "Row 2");
    }
    void LogDebug(const Matrix4& m, const char* msg)
    {
        Logger::LogDebug("%s:", msg);
        LogDebug(m.rows[0], "Row 0");
        LogDebug(m.rows[1], "Row 1");
        LogDebug(m.rows[2], "Row 2");
        LogDebug(m.rows[3], "Row 3");
    }

    void LogDebug(const math::Quaternion& q, const char* msg)
    {
        Logger::LogDebug("%s: Scalar: %f", msg, q.scalar);
        LogDebug(q.vector, "Vector");
    }

    void LogDebug(const Transform& t, const char* msg)
    {
        Logger::LogDebug(msg);
        LogDebug(t.model, "Model");
        LogDebug(t.position, "Position");
        LogDebug(t.rotation, "Rotation");
        LogDebug(t.scale, "Scale");
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
