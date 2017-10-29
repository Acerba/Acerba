#pragma once

#include <Ace/Platform.h>

#if ACE_DEBUG

#include <Ace/TypesFwd.h>

namespace ace
{

    struct AABB;
    class Transform;

    void LogDebug(const float, const char* msg = "");
    void LogDebug(const AABB&, const char* msg = "");
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
