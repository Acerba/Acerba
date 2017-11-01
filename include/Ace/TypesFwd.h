
#pragma once

#include <utilities/Types.hpp>

namespace ace
{
    namespace math
    {
        using Vector2 = mv::Vektor<2u, float>;
        using Vector3 = mv::Vektor<3u, float>;
        using Vector4 = mv::Vektor<4u, float>;

        using Matrix2 = mv::Mat<2u, 2u, float>;
        using Matrix3 = mv::Mat<3u, 3u, float>;
        using Matrix4 = mv::Mat<4u, 4u, float>;

        using Quaternion = mv::Quat<float>;
    }
}