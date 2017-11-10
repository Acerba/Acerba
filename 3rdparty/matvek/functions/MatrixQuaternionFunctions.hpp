/// MATVEK C++ Library
/// Copyright (c) 2017 Eetu Asikainen
/// 
/// This software is provided 'as-is', without any express or implied
/// warranty. In no event will the authors be held liable for any damages
/// arising from the use of this software.
/// 
/// Permission is granted to anyone to use this software for any purpose,
/// including commercial applications, and to alter it and redistribute it
/// freely, subject to the following restrictions:
/// 
/// 1. The origin of this software must not be misrepresented; you must not
///    claim that you wrote the original software. If you use this software
///    in a product, an acknowledgement in the product documentation would be
///    appreciated but is not required.
/// 2. Altered source versions must be plainly marked as such, and must not be
///    misrepresented as being the original software.
/// 3. This notice may not be removed or altered from any source distribution.

#ifndef MV_MQF_HPP
#define MV_MQF_HPP

#if MV_EXPERIMENTAL
//Experimental
//Returns derivative of a quaternion operator
template <typename Type>
static MV_API Mat<3u, 3u, Type> DerivativeOfQuaternionOperator(const Quat<Type>& q)
{
    const Type t = static_cast<Type>(2);
    const Vektor<3u, Type> w(AngularRateVektor(q._v));
    const Mat<3u, 3u, Type> T(ToMatrix(q));
    return Transpose(Mat<3u,3u,Type>(
        Cross(Vektor<3u, Type>(T.at(0, 0)*t, T.at(1, 0)*t, T.at(2, 0)*t), w),
        Cross(Vektor<3u, Type>(T.at(0, 1)*t, T.at(1, 1)*t, T.at(2, 1)*t), w),
        Cross(Vektor<3u, Type>(T.at(0, 2)*t, T.at(1, 2)*t, T.at(2, 2)*t), w)
    ));
}
//////////////////////////////////////////////////////////
#endif


//Converts quaternion to equivalent matrix
template <UInt8 Size, typename Type>
static MV_API Mat<Size, Size, Type> ToMatrix(const Quat<Type>& q)
{
    const Type o = static_cast<Type>(1);
    const Type t = static_cast<Type>(2);
    const Quat<Type> u(GetUnitQuat(q));
    return ResizeMatrix<Size, Size>(
        Mat<3u, 3u, Type>(
            t*(u.S*u.S + u.X*u.X) - o, t*(u.X*u.Y - u.S*u.Z), t*(u.X*u.Z + u.S*u.Y),
            t*(u.X*u.Y + u.S*u.Z), t*(u.S*u.S + u.Y*u.Y) - o, t*(u.Y*u.Z - u.S*u.X),
            t*(u.X*u.Z - u.S*u.Y), t*(u.Y*u.Z + u.S*u.X), t*(u.S*u.S + u.Z*u.Z) - o
        )
    );
}
//////////////////////////////////////////////////////////


//Returns quaternion
template <typename Type>
static MV_API Quat<Type> ToQuat(const Mat<3u, 3u, Type>& m)
{
    using math::Sqrt;
    
    // g++ 7.0.1 not constexpr otherwise
    auto sq = m[0] + m.template get<1, 1>() + m.template get<2, 2>();
    if MV_API (sq > 0)
    {
        const MV_TYPE s = static_cast<MV_TYPE>(0.5f) / Sqrt(sq + static_cast<MV_TYPE>(1.f));
        return Quat<Type>(
            static_cast<Type>(0.25f / s),
            static_cast<Type>((m.at(2, 1) - m.at(1, 2)) * s),
            static_cast<Type>((m.at(0, 2) - m.at(2, 0)) * s),
            static_cast<Type>((m.at(1, 0) - m.at(0, 1)) * s)
        );
    }
    else
    {
        if MV_API (m[0] > m.at(1, 1) && m[0] > m.at(2, 2))
        {
            const MV_TYPE s = 2.0f * Sqrt(1.0f + m[0] - m.at(1, 1) - m.at(2, 2));
            return Quat<Type>(
                static_cast<Type>((m.at(2, 1) - m.at(1, 2)) / s),
                static_cast<Type>(0.25f * s),
                static_cast<Type>((m.at(0, 1) + m.at(1, 0)) / s),
                static_cast<Type>((m.at(0, 2) + m.at(2, 0)) / s)
            );
        }
        else if MV_API (m.at(1, 1) > m.at(2, 2))
        {
            const MV_TYPE s = 2.0f * Sqrt(1.0f + m.at(1, 1) - m[0] - m.at(2, 2));
            return Quat<Type>(
                static_cast<Type>((m.at(0, 2) - m.at(2, 0)) / s),
                static_cast<Type>((m.at(0, 1) + m.at(1, 0)) / s),
                static_cast<Type>(0.25f * s),
                static_cast<Type>((m.at(1, 2) + m.at(2, 1)) / s)
            );
        }
        else
        {
            const MV_TYPE s = 2.0f * Sqrt(1.0f + m.at(2, 2) - m[0] - m.at(1, 1));
            return Quat<Type>(
                static_cast<Type>((m.at(1, 0) - m.at(0, 1)) / s),
                static_cast<Type>((m.at(0, 2) + m.at(2, 0)) / s),
                static_cast<Type>((m.at(1, 2) + m.at(2, 1)) / s),
                static_cast<Type>(0.25f * s)
            );
        }
    }
}
//////////////////////////////////////////////////////////


template <typename Type>
static MV_API Quat<Type> LookAt(const Vektor<3u, Type>& forward, const Vektor<3u, Type>& up)
{
    const Vektor<3u, Type> a(Normalize(forward));
    const Vektor<3u, Type> b(Normalize(Cross(up, a)));
    const Vektor<3u, Type> c(Cross(a, b));
    
    return ToQuat(
        Mat<3u, 3u, Type>(
            b.x, b.y, b.z,
            c.x, c.y, c.z,
            a.x, a.y, a.z
        )
    );
}
//////////////////////////////////////////////////////////


template <typename Type>
static MV_API Quat<Type> LookAt2(const Vektor<3u, Type>& forward, const Vektor<3u, Type>& up)
{
    const Vektor<3u, Type> v1(Normalize(forward));
    const Vektor<3u, Type> v2(Normalize(Cross(up, v1)));
    const Vektor<3u, Type> v3(Cross(v1, v2));
    
    const Type m00 = v2.x;
    const Type m01 = v2.y;
    const Type m02 = v2.z;
    const Type m10 = v3.x;
    const Type m11 = v3.y;
    const Type m12 = v3.z;
    const Type m20 = v1.x;
    const Type m21 = v1.y;
    const Type m22 = v1.z;


    const Type num8 = (m00 + m11) + m22;
    Quat<Type> q;

    if (num8 > static_cast<Type>(0))
    {
        Type num = math::Sqrt(num8 + static_cast<Type>(1));
        q.S = num * static_cast<Type>(0.5f);
        num = static_cast<Type>(0.5f) / num;
        q.X = (m12 - m21) * num;
        q.Y = (m20 - m02) * num;
        q.Z = (m01 - m10) * num;
    }
    else if ((m00 >= m11) && (m00 >= m22))
    {
        const Type num7 = math::Sqrt(((static_cast<Type>(1) + m00) - m11) - m22);
        const Type num4 = static_cast<Type>(0.5f) / num7;
        q.X = static_cast<Type>(0.5f) * num7;
        q.Y = (m01 + m10) * num4;
        q.Z = (m02 + m20) * num4;
        q.S = (m12 - m21) * num4;
    }
    else if (m11 > m22)
    {
        const Type num6 = math::Sqrt(((static_cast<Type>(1) + m11) - m00) - m22);
        const Type num3 = static_cast<Type>(0.5f) / num6;
        q.X = (m10 + m01) * num3;
        q.Y = static_cast<Type>(0.5f) * num6;
        q.Z = (m21 + m12) * num3;
        q.S = (m20 - m02) * num3;
    }
    else
    {
        const Type num5 = math::Sqrt(((static_cast<Type>(1) + m22) - m00) - m11);
        const Type num2 = static_cast<Type>(0.5f) / num5;
        q.X = (m20 + m02) * num2;
        q.Y = (m21 + m12) * num2;
        q.Z = static_cast<Type>(0.5f) * num5;
        q.S = (m01 - m10) * num2;
    }
    return q;
}
//////////////////////////////////////////////////////////


#endif // MV_MQF_HPP
