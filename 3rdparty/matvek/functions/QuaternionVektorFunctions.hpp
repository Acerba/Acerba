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

#ifndef MV_QVF_HPP
#define MV_QVF_HPP


//Returns composite axis around which this rotation quaternion is happening
template <typename Type>
static MV_API Vektor<3u, Type> GetCompositeRotationAxis(const Quat<Type>& q)
{
    return Vektor<3u, Type>(q._v);
}
//////////////////////////////////////////////////////////


//Returns rotations from a quaternion
//Pitch Yaw, Roll (XYZ)
template <typename Type>
static MV_API Vektor<3u, Type> GetRotations(const Quat<Type>& q)
{
    const Type o = static_cast<Type>(1);
    const Type t = static_cast<Type>(2);
    return Vektor<3u, Type>(
        math::ATan2(t*(q.S*q.X + q.Y*q.Z), o - t*(q.X*q.X + q.Y*q.Y)),
        math::ASin(t*(q.S*q.Y - q.Z*q.X)),
        math::ATan2(t*(q.S*q.Z + q.X*q.Y), o - t*(q.Y*q.Y + q.Z*q.Z))
    );
}
//////////////////////////////////////////////////////////


#if MV_EXPERIMENTAL
//Combined rotation quaternion
//Roll, Pitch, Roll (ZXZ)
//Longitude, Latitude, Track direction
template <typename Type>
static MV_API Quat<Type> MakeCompositeOrbitEphemerisRotationQ(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    
    const Type t = static_cast<Type>(2);
    //mu, epsilon, ro
    const Type cm = Cos(rads[0] / t);
    const Type sm = Sin(rads[0] / t);
    const Type ce = Cos(rads[1] / t);
    const Type se = Sin(rads[1] / t);
    const Type cr = Cos(rads[2] / t);
    const Type sr = Sin(rads[2] / t);
    
    return Quat<Type>(
        cm*ce*cr - sm*se*sr,
        cm*ce*sr + sm*se*cr,
        sm*ce*sr - cm*se*cr,
        cm*se*sr + sm*ce*cr
    );
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Combined rotation quaternion
//Roll, Pitch, Roll (ZXZ)
//to Ascending node, Orbit inclination, Argument of Latitude
template <typename Type>
static MV_API Quat<Type> MakeCompositeOrbitRotationQ(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    
    //omega, beta, gamma
    const Type co = Cos(rads[0] * static_cast<Type>(0.5f));
    const Type so = Sin(rads[0] * static_cast<Type>(0.5f));
    const Type cb = Cos(rads[1] * static_cast<Type>(0.5f));
    const Type sb = Sin(rads[1] * static_cast<Type>(0.5f));
    const Type cg = Cos(rads[2] * static_cast<Type>(0.5f));
    const Type sg = Sin(rads[2] * static_cast<Type>(0.5f));
    
    return Quat<Type>(
        co*cb*cg - so*cb*sg,
        co*sb*cg + so*sb*sg,
        -co*sb*sg + so*sb*cg,
        co*cb*sg + so*cb*cg
    );
}
//////////////////////////////////////////////////////////
#endif


//Combined rotation quaternion
//Pitch, Yaw, Roll (XYZ)
template <typename Type>
static MV_API Quat<Type> MakeCompositeRotationQ(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    
    //pitch, yaw, roll
    const Type cp = Cos(rads[0] * static_cast<Type>(0.5f));
    const Type sp = Sin(rads[0] * static_cast<Type>(0.5f));
    const Type cy = Cos(rads[1] * static_cast<Type>(0.5f));
    const Type sy = Sin(rads[1] * static_cast<Type>(0.5f));
    const Type cr = Cos(rads[2] * static_cast<Type>(0.5f));
    const Type sr = Sin(rads[2] * static_cast<Type>(0.5f));
    
    return Quat<Type>(
        cr*cy*cp + sr*sy*sp,
        cr*cy*sp - sr*sy*cp,
        cr*sy*cp + sr*cy*sp,
        sr*cy*cp - cr*sy*sp
    );
}
//////////////////////////////////////////////////////////

template <typename Type>
static MV_API Quat<Type> MakeQuaternion(const Vektor<3u, Type>& angles, const bool rads)
{
    const Type x = (rads ? angles.x : ToRad(angles.x)) / static_cast<Type>(2);
    const Type y = (rads ? angles.y : ToRad(angles.y)) / static_cast<Type>(2);
    const Type z = (rads ? angles.z : ToRad(angles.z)) / static_cast<Type>(2);

    return (
        Quat<Type>(math::Cos(x), math::Sin(x), static_cast<Type>(0), static_cast<Type>(0)) *
        Quat<Type>(math::Cos(y), static_cast<Type>(0), math::Sin(y), static_cast<Type>(0)) *
        Quat<Type>(math::Cos(z), static_cast<Type>(0), static_cast<Type>(0), math::Sin(z))
    );
}

//Returns rotation quaternion rad radians around axis v
template <typename Type>
static MV_API Quat<Type> MakeRotationQ(const Vektor<3u, Type>& v, const Type rad)
{
    return Quat<Type>(
        math::Cos(rad / static_cast<Type>(2)),
        mv::GetUnitVektor(v) * math::Sin(rad / static_cast<Type>(2))
    );
}
//////////////////////////////////////////////////////////


//Rotates v around a rotation of q
template <typename Type>
static MV_API Vektor<3u, Type> operator*(Quat<Type> q, const Vektor<3u, Type>& v)
{
    return ((q * Quat<Type>(v)) * Inverse(q))._v;
}
//////////////////////////////////////////////////////////


template <typename Type>
static MV_API Vektor<3u, Type> RotateAroundAxis(const Quat<Type>& rot, const Vektor<3u, Type>& point)
{
    return rot * point;
}
//////////////////////////////////////////////////////////


template <typename Type>
static MV_API Vektor<3u, Type> RotateAroundAxis(const Vektor<3u, Type>& axis, const Vektor<3u, Type>& point, const Type rad)
{
    return (
        Quat<Type>(
            math::Cos(rad / static_cast<Type>(2)),
            GetUnitVektor(axis) * math::Sin(rad / static_cast<Type>(2))
        ) *
        point
    );
}
//////////////////////////////////////////////////////////


#endif // MV_QVF_HPP
