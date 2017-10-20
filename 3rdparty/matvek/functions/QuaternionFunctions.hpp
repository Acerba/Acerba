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

#ifndef MV_QF_HPP
#define MV_QF_HPP



//Get complex conjugate (keeps S, inverts V)
template <typename Type>
static MV_API Quat<Type> ComplexConjugate(const Quat<Type>& q)
{
    return Quat<Type>(q.S, -q.X, -q.Y, -q.Z);
}
//////////////////////////////////////////////////////////


#if MV_EXPERIMENTAL
//Experimental
//Returns derivative of quaternion
template <typename Type>
static MV_API Quat<Type> DerivativeOfQuaternion(const Quat<Type>& q)
{
    return q * AngularRateVektor(q._v);
}
//////////////////////////////////////////////////////////
#endif

#if MV_EXPERIMENTAL
//Experimental
//Returns derivative of quaternions conjugate (don't pass conjugate in)
template <typename Type>
static MV_API Quat<Type> DerivativeOfQuaternionConjugate(const Quat<Type>& q)
{
    return ComplexConjugate(q) * -AngularRateVektor(q._v);
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Experimental
//Decomposes result into return value
//Un-normalizes result (does this have any applications)
template <typename Type>
static MV_API Quat<Type> FactorsDistinctPrincipalAxis(const Quat<Type>& result)
{
    const Quat<Type> p(GetUnitQuat(result));
    const Type A = p.S*p.X + p.Y*p.Z;
    const Type B = -p.S*p.S + p.Y*p.Y;
    const Type D = p.X*p.X - (p.Z*p.Z);
    const Type theta = math::ATan(-(static_cast<Type>(2)*A) / (B + D));
    const Quat<Type> c(
        math::Cos(theta / static_cast<Type>(2)),
        math::Sin(theta / static_cast<Type>(2)),
        static_cast<Type>(0),
        static_cast<Type>(0)
    );
    return Quat<Type>(
        p.S*c.S + p.X*c.X,
        p.X*c.S - p.S*c.X,
        p.Y*c.S - p.Z*c.X,
        p.Z*c.S + p.Y*c.X
    );
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Experimental
template <typename Type>
static MV_API Quat<Type> FactorsRepeatedPrincipalAxis(const Quat<Type>& result)
{
    const Quat<Type> p(GetUnitQuat(result));
    const Type S = -p.S*p.X - p.S*p.Y;
    const Type T = -p.Y*p.Z - p.Y*p.S;
    const Type U = p.X*(p.S + p.Z);
    const Type V = p.Z*(p.X + p.Y);
    
    const Type o = static_cast<Type>(1);
    const Type e = static_cast<Type>(2);
    
    const Type part1 = (S + V) / (e*T);
    const Type part2 = math::Sqrt(o - (U*T*e*e) / (math::Pow(S + V, e)));
    
    const Type res1 = part1 * (-o + part2);
    const Type res2 = part1 * (-o - part2);
    
    
    //a = resx * b
    //b = a / resx
    //sqrt(a^2 + (a/resx)^2) = 1 = sqrt(b^2 + (resx*b)^2)
    //-> 8 solutions for res1
    //-> 8 solutions for res2
    //Correct res?
    
    return result;
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Experimental
//Decomposes result into q1 and q2 which make up result (in standard reference frame)
//Use empty pre-initialized quaternions for q1 and q2 for all data in them will be overwritten
template <typename Type>
static MV_API void FactorsThreePrincipalAxis(const Quat<Type>& result, Quat<Type>& q1, Quat<Type>& q2)
{
    const Type at = math::ATan(result.Y / result.S);
    
    q1.S = math::Cos(at);
    q1.X = static_cast<Type>(0);
    q1.Y = math::Sin(at);
    q1.Z = static_cast<Type>(0);
    
    q2.S = q1.S*result.S + q1.Y*result.Y;
    q2.X = q1.S*result.X - q1.Y*result.Z;
    q2.Y = static_cast<Type>(0);
    q2.Z = q1.Y*result.X + q1.S*result.Z;
}
//////////////////////////////////////////////////////////
#endif


//Returns total rotation around the composite axis
template <typename Type>
static MV_API Type GetCompositeRotationAngle(const Quat<Type>& q)
{
    return static_cast<Type>(2)*math::ACos(q.S);
}
//////////////////////////////////////////////////////////


//Divides q by its length, no effect if already normalized
template <typename Type>
static MV_API Quat<Type> GetUnitQuat(const Quat<Type>& q)
{
    Type l = Length(q);
    if MV_API (math::Epsilon(l))
    {
        MV_ASSERT(false, "Quaternion division by zero");
        l = static_cast<Type>(detail::MV_BOUNDARY);
    }
    return Quat<Type>(q.S / l, q.X / l, q.Y / l, q.Z / l);
}
//////////////////////////////////////////////////////////


//Returns SLERP-interpolated position between positions q1 and q2, from a timeframe of t (0...1)
template <typename Type>
static MV_API Quat<Type> InterpolateSLERP(const Quat<Type>& q1, const Quat<Type>& q2, const Type t)
{
    using math::Sin;
    
    if (t <= static_cast<Type>(0)) return q1;
    if (t >= static_cast<Type>(1)) return q2;
    
    const Type theta = math::ACos(
        (q1.S*q2.S + q1.X*q2.X + q1.Y*q2.Y + q1.Z*q2.Z) /
        (q1.length() * q2.length())
    );
    
    return (
        (q1 * ((Sin((static_cast<Type>(1) - t) * theta)) / (Sin(theta)))) +
        (q2 * ((Sin(t * theta)) / (Sin(theta))))
    );
}
//////////////////////////////////////////////////////////


//Invert a quaternion (get complex conjugate (keeps S, inverts V))
template <typename Type>
static MV_API Quat<Type> Inverse(const Quat<Type>& q)
{
    return ComplexConjugate(q) * (static_cast<Type>(1) / (Length(q) * Length(q)));
}
//////////////////////////////////////////////////////////


//Norm
template <typename Type>
static MV_API Type Length(const Quat<Type>& q)
{
    return math::Sqrt(q.S*q.S + q.X*q.X + q.Y*q.Y + q.Z*q.Z);
}
//////////////////////////////////////////////////////////


template <typename Type>
static MV_API Quat<Type> operator*(Quat<Type> q, const Type scalar)
{
    q *= scalar;
    return q;
}
//////////////////////////////////////////////////////////


#if MV_DEBUG
//Print quaternion
template <typename Type>
static MV_API void Print(const Quat<Type>& q)
{
    _msg(q.S, ',', q.X, ',', q.Y, ',', q.Z, '\n');
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Experimental
template <typename Type>
static MV_API Quat<Type> QuaternionPerturbation(Quat<Type>& q)
{
    if (q.S == static_cast<Type>(0))
    {
        return q;
    }
    
    MV_ASSERT(false, "Math under construction");
    return q;
}
//////////////////////////////////////////////////////////
#endif


template <typename Type>
static MV_API Quat<Type> Scale(Quat<Type> copy, const Type scalar)
{
    copy.scale(scalar);
    return copy;
}
//////////////////////////////////////////////////////////


#endif // MV_QF_HPP
