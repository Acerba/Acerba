#pragma once

#include <Ace/Platform.h>

#if ACE_DEBUG

namespace ace
{
    namespace math
    {
        class Matrix2;
        class Matrix3;
        class Matrix4;
        class Quaternion;
        class Vector2;
        class Vector3;
        class Vector4;
    }
    class Transform;

    void LogDebug(const float, const char* msg = "");
    void LogDebug(const math::Matrix2&, const char* msg = "");
    void LogDebug(const math::Matrix3&, const char* msg = "");
    void LogDebug(const math::Matrix4&, const char* msg = "");
    void LogDebug(const math::Quaternion&, const char* msg = "");
    void LogDebug(const Transform& t, const char* msg = "");
    void LogDebug(const math::Vector2&, const char* msg = "");
    void LogDebug(const math::Vector3&, const char* msg = "");
    void LogDebug(const math::Vector4&, const char* msg = "");

}

#endif // ACE_DEBUG
