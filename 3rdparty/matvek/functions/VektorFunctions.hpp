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

#ifndef MV_VF_HPP
#define MV_VF_HPP

//Returns copy of v with each element gone through abs
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Absolute(Vektor<Size, Type> copy)
{
    copy.absolute();
    return copy;
}
//////////////////////////////////////////////////////////


#if MV_EXPERIMENTAL
template <UInt16 Size, typename Type>
static MV_API Type AngularRateAboutVektor(const Vektor<Size, Type>& v)
{
    MV_ASSERT(false, "Math under construction");
    return static_cast<Type>(1);
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> AngularRateVektor(const Vektor<Size, Type>& v)
{
    return v * AngularRateAboutVektor(v);
}
//////////////////////////////////////////////////////////
#endif

//Rotates target rad radians around axis
template <typename Type>
static MV_API Vektor<3u, Type> AxisAngleRotation(const Vektor<3u, Type>& target, const Vektor<3u, Type>& axis, const Type rad)
{
    const Vektor<3u, Type> n(GetUnitVektor(axis));
    return Vektor<3u, Type>(
        (target * math::Cos(rad)) +
        ((n * Dot(target, n)) * (static_cast<Type>(1) - math::Cos(rad))) +
        (Cross(n, target) * math::Sin(rad))
    );
}
//////////////////////////////////////////////////////////


//Returns a vektor which has the components of two vektors multiplied
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> ComponentProduct(Vektor<Size, Type> copy, const Vektor<Size, Type>& v2)
{
    for(UInt16 i = 0u; i < Size; ++i)
    {
        copy[i] *= v2[i];
    }
    return copy;
}
//////////////////////////////////////////////////////////


//Returns a copy of the vektor
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Copy(const Vektor<Size, Type>& v)
{
    return Vektor<Size, Type>(v);
}
//////////////////////////////////////////////////////////


//Returns cross product of two vektors (only applicable on vektors with a size of 2)
template <typename Type>
static MV_API Type Cross(const Vektor<2u, Type>& v1, const Vektor<2u, Type>& v2)
{
    return (v1.x * v2.y) - (v1.y * v2.x);
}
//////////////////////////////////////////////////////////


//Returns cross product of two vektors (only applicable on vektors with a size of 3)
template <typename Type>
static MV_API Vektor<3u, Type> Cross(const Vektor<3u, Type>& v1, const Vektor<3u, Type>& v2)
{
    return Vektor<3u, Type>(
        ((v1[1] * v2[2]) - (v1[2] * v2[1])),
        -((v1[0] * v2[2]) - (v1[2] * v2[0])),
        ((v1[0] * v2[1]) - (v1[1] * v2[0]))
    );
}
//////////////////////////////////////////////////////////


//Returns dot product of two vektors
template <UInt16 Size, typename Type>
static MV_API Type Dot(const Vektor<Size, Type>& v1, const Vektor<Size, Type>& v2)
{
    Type res = static_cast<Type>(0);
    for (UInt16 i = 0u; i < Size; ++i)
    {
        res += v1[i] * v2[i];
    }
    return res;
}
//////////////////////////////////////////////////////////


//Returns angle between two vektors in radians
template <UInt16 Size, typename Type>
static MV_API Type GetAngleBetween(const Vektor<Size, Type>& v1, const Vektor<Size, Type>& v2)
{
    return math::ACos(Dot(v1, v2) / (v1.length() * v2.length()));
}
//////////////////////////////////////////////////////////


//Returns radians of angle
template <typename Type>
static MV_API Type GetDirectionCosine(const Vektor<3u, Type>& v, const AXIS axis)
{
    switch (axis)
    {
        case AXIS::X: return static_cast<Type>(math::ACos(v[0] / v.length()));
        case AXIS::Y: return static_cast<Type>(math::ACos(v[1] / v.length()));
        case AXIS::Z: return static_cast<Type>(math::ACos(v[2] / v.length()));
        default: MV_ASSERT(false, "Invalid rotation angle");
    }
    return static_cast<Type>(0);
}
//////////////////////////////////////////////////////////


//Returns unitvektor of v as new vektor
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> GetUnitVektor(Vektor<Size, Type> copy)
{
    copy.normalize();
    return copy;
}
//////////////////////////////////////////////////////////


//Linear interpolation between v1 and v2 in a timeframe t 0...1
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> InterpolateLERP(const Vektor<Size, Type>& v1, const Vektor<Size, Type>& v2, const MV_TYPE t)
{
    return (
        t <= static_cast<MV_TYPE>(0) ? v1 :
        t >= static_cast<MV_TYPE>(1) ? v2 :
        Vektor<Size, Type>(v1 + ((v2 - v1) * t))
    );
}
//////////////////////////////////////////////////////////


//Spherical linear interpolation between v1 and v2 in a timeframe t 0...1
//Uses linear interpolation if theta is very small
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> InterpolateSLERP(const Vektor<Size, Type>& v1, const Vektor<Size, Type>& v2, const MV_TYPE t)
{
    const Type theta = GetAngleBetween(v1, v2);
    return (
        t <= static_cast<MV_TYPE>(0) ? v1 :
        t >= static_cast<MV_TYPE>(0) ? v2 :
        math::Epsilon(theta) ?
        InterpolateLERP(v1, v2, t) :
        Vektor<Size, Type>(
            (v1 * ((math::Sin((static_cast<MV_TYPE>(1) - t) * theta)) / (math::Sin(theta)))) +
            (v2 * ((math::Sin(t * theta)) / (math::Sin(theta))))
        )
    );
}
//////////////////////////////////////////////////////////


// Checks if all elements in target are within min and max
template <UInt16 Size, typename Type>
static MV_API bool IsWithin(const Vektor<Size, Type>& target, const Vektor<Size, Type>& min, const Vektor<Size, Type>& max)
{
    return target.isWithin(min, max);
}
//////////////////////////////////////////////////////////


// Checks if all elements in target are +- offset of minmax
template <UInt16 Size, typename Type>
static MV_API bool IsWithin(const Vektor<Size, Type>& target, const Vektor<Size, Type>& minmax, const Type& offset)
{
    return target.isWithin(minmax, offset);
}
//////////////////////////////////////////////////////////


//Returns length of a vektor
template <UInt16 Size, typename Type>
static MV_API Type Length(const Vektor<Size, Type>& v)
{
    return v.length();
}
//////////////////////////////////////////////////////////


//Returns squared length of a vektor
template <UInt16 Size, typename Type>
static MV_API Type LengthSquared(const Vektor<Size, Type>& v)
{
    return v.lengthSquared();
}
//////////////////////////////////////////////////////////


//Modifies A and B so that they are perpendicular, and return C, which is perpendicular to both A and B. Direction of A will not change.
template <typename Type>
static MV_API Vektor<3u, Type> MakeOrthoNormal(Vektor<3u, Type>& a, Vektor<3u, Type>& b)
{
    a.normalize();
    Vektor<3u, Type> c(Cross(a, b));
    if(math::Epsilon(c.lengthSquared()))
    {
        MV_ASSERT(false, "Tried to orthonormalize vektors that are parallel");
        return c;
    }
    c.normalize();
    b = Cross(c, a);
    return c;
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type, typename ... Args>
static MV_API Vektor<Size, Type> MakeVektor(Args ... args)
{
    static_assert(Size != 0u, "MakeVektor: Cannot create vektor with dimension 0");
    static_assert(sizeof...(Args) == Size, "MakeVektor: Argument count does not match Size");
    return Vektor<Size, Type>(static_cast<Type>(args)...);
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Normalize(Vektor<Size, Type> copy)
{
    copy.normalize();
    return copy;
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type>
MV_API Vektor<Size, Type>& operator*=(Vektor<Size, Type>& lhs, const Type& rhs)
{
    for (UInt16 i = 0u; i < Size; ++i)
    {
        lhs[i] *= rhs;
    }
    return lhs;
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type>
MV_API Vektor<Size, Type> operator*(Vektor<Size, Type> lhs, const Type& rhs)
{
    lhs *= rhs;
    return lhs;
}
//////////////////////////////////////////////////////////


#if MV_DEBUG
//Print vektor
template <UInt16 Size, typename Type>
static MV_API void Print(const Vektor<Size, Type>& v)
{
    for(const auto& itr : v.data())
    {
        _msg(itr, ',');
    }
    _msg('\n');
}
//////////////////////////////////////////////////////////
#endif


// Project a onto b
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Project(const Vektor<Size, Type>& a, const Vektor<Size, Type>& b)
{
    return b * (Dot(a, b) / b.lengthSquared());
}
//////////////////////////////////////////////////////////

#if !MV_CONSTEXPR

namespace detail
{
    template <UInt16 NewSize>
    struct ResizeVektorHelper final
    {
        template <typename Type, UInt16 OldSize, typename std::enable_if<(NewSize == OldSize)>::type* = nullptr>
        static Vektor<NewSize, Type> ResizeVektor(const Vektor<OldSize, Type>& v)
        {
            return v;
        }
        template <typename Type, UInt16 OldSize, typename std::enable_if<(NewSize < OldSize)>::type* = nullptr>
        static Vektor<NewSize, Type> ResizeVektor(const Vektor<OldSize, Type>& v)
        {
            std::array<Type, NewSize> arr;
            for (UInt16 i = 0u; i < NewSize; ++i)
            {
                arr[i] = v[i];
            }
            return Vektor<NewSize, Type>(arr);
        }
        template <typename Type, UInt16 OldSize, typename std::enable_if<(NewSize > OldSize)>::type* = nullptr>
        static Vektor<NewSize, Type> ResizeVektor(const Vektor<OldSize, Type>& v)
        {
            std::array<Type, NewSize> arr;
            for (UInt16 i = 0u; i < OldSize; ++i)
            {
                arr[i] = v[i];
            }
            for (UInt16 i = OldSize; i < NewSize; ++i)
            {
                arr[i] = static_cast<Type>(0);
            }
            return Vektor<NewSize, Type>(arr);
        }
    };
}

#endif // !MV_CONSTEXPR

template <UInt16 NewSize, typename Type, UInt16 OldSize>
static MV_API Vektor<NewSize, Type> ResizeVektor(const Vektor<OldSize, Type>& v)
{
    #if MV_CONSTEXPR
    if MV_API (NewSize == OldSize)
    {
        return v;
    }
    else if MV_API (NewSize < OldSize)
    {
        std::array<Type, NewSize> arr;
        for (UInt16 i = 0u; i < NewSize; ++i)
        {
            arr[i] = v[i];
        }
        return Vektor<NewSize, Type>(arr);
    }
    else
    {
        std::array<Type, NewSize> arr;
        for (UInt16 i = 0u; i < OldSize; ++i)
        {
            arr[i] = v[i];
        }
        for (UInt16 i = OldSize; i < NewSize; ++i)
        {
            arr[i] = static_cast<Type>(0);
        }
        return Vektor<NewSize, Type>(arr);
    }
    #else
        return detail::ResizeVektorHelper<NewSize>::ResizeVektor(v);
    #endif
}


template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Invert(Vektor<Size, Type> copy)
{
    copy.invert();
    return copy;
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Scale(Vektor<Size, Type> copy, const Type scalar)
{
    copy.scale(scalar);
    return copy;
}
//////////////////////////////////////////////////////////


//Multiplies every element in v1 with the corresponding element in v2
template <UInt16 Size, typename Type>
static MV_API Vektor<Size, Type> Scale(Vektor<Size, Type> copy, const Vektor<Size, Type>& scalars)
{
    for (UInt16 i = 0u; i < Size; ++i)
    {
        copy[i] *= scalars[i];
    }
    return copy;
}
//////////////////////////////////////////////////////////


#endif // MV_VF_HPP
