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

#ifndef MV_MVF_HPP
#define MV_MVF_HPP



template <UInt8 Rows, UInt8 Cols, typename Type, UInt16 Size>
static MV_API Mat<Rows, Cols, Type> ToMatrix(const Vektor<Size, Type>& v)
{
    static_assert(
        (Rows == 1u && Cols == Size) || (Cols == 1u && Rows == Size),
        "ToMatrix: One matrix dimension must be 1, and the other must match Vektor size"
    );
    return Mat<Rows, Cols, Type>(v.data());
}

#if !MV_CONSTEXPR
namespace detail
{
    template <bool>
    struct ToVektorHelper final
    {
        // Row by row
        MV_DISABLE_CLASS(ToVektorHelper);

        template <UInt8 Rows, UInt8 Cols, typename Type>
        static Vektor<Rows * Cols, Type> ToVektor(const Mat<Rows, Cols, Type>& m)
        {
            std::array<Type, Rows * Cols> arr;
            for (UInt16 i = 0u; i < (Rows * Cols); ++i)
            {
                arr[i] = m[i];
            }
            return Vektor<Rows * Cols, Type>(arr);
        }
    };
    template <>
    struct ToVektorHelper<false> final
    {
        // Column by column
        MV_DISABLE_CLASS(ToVektorHelper);

        template <UInt8 Rows, UInt8 Cols, typename Type>
        static Vektor<Rows * Cols, Type> ToVektor(const Mat<Rows, Cols, Type>& m)
        {
            std::array<Type, Rows * Cols> arr;
            UInt16 index = 0u;
            for (UInt8 j = 0u; j < Cols; ++j)
            {
                for (UInt8 i = 0u; i < Rows; ++i)
                {
                    arr[index] = m.at(i, j);
                    ++index;
                }
            }
            return Vektor<Rows * Cols, Type>(arr);
        }
    };
} // detail
#endif // !MV_CONSTEXPR ToVektorHelper

template <bool FlattenByRow = true, UInt8 Rows, UInt8 Cols, typename Type>
static MV_API Vektor<Rows * Cols, Type> ToVektor(const Mat<Rows, Cols, Type>& m)
{
    #if MV_CONSTEXPR
    std::array<Type, Rows * Cols> arr;
    if MV_API (FlattenByRow) // Row by row
    {
        for (UInt8 i = 0u; i < Rows; ++i)
        {
            for (UInt8 j = 0u; j < Cols; ++i)
            {
                arr[(i * Cols) + j] = m.at(i, j);
            }
        }
    }
    else // Column by column
    {
        UInt16 index = 0u;
        for (UInt8 j = 0u; j < Cols; ++j)
        {
            for (UInt8 i = 0u; i < Rows; ++i)
            {
                arr[index] = m.at(i, j);
                ++index;
            }
        }
    }
    return Vektor<Rows * Cols, Type>(arr);
    #else
    return detail::ToVektorHelper<FlattenByRow>::ToVektor(m);
    #endif
}



//Cross product between Mat3 and Vek3
template <typename Type>
static MV_API Mat<3u, 3u, Type> Cross(const Mat<3u, 3u, Type>& m, const Vektor<3u, Type>& v)
{
    return m * Tilde(v);
}
//////////////////////////////////////////////////////////


//Cross product between Vek3 and Mat3
template <typename Type>
static MV_API Mat<3u, 3u, Type> Cross(const Vektor<3u, Type>& v, const Mat<3u, 3u, Type>& m)
{
    return Tilde(v) * m;
}
//////////////////////////////////////////////////////////


//Returns composite axis around which this rotation matrix is happening
template <typename Type>
static MV_API Vektor<3u, Type> GetCompositeRotationAxis(const Mat<3u, 3u, Type>& m)
{
    return Vektor<3u, Type>(
        // m.at(1, 2), m.at(2, 0), m.at(0, 1)
        m.template get<1u, 2u>() - m.template get<2u, 1u>(),
        m.template get<2u, 0u>() - m.template get<0u, 2u>(),
        m.template get<0u, 1u>() - m.template get<1u, 0u>()
    );
}
//////////////////////////////////////////////////////////


template <typename Type>
static MV_API Mat<4u, 4u, Type> LookAt(const Vektor<3u, Type>& eye, const Vektor<3u, Type>& target, const Vektor<3u, Type>& up)
{
    const Vektor<3u, Type> Z(GetUnitVektor(target - eye));
    const Vektor<3u, Type> X(GetUnitVektor(Cross(up, Z)));
    const Vektor<3u, Type> Y(Cross(Z, X));
    
    return MakeMatrix<4u, 4u, Type>(
        X.template get<0>(), Y.template get<0>(), Z.template get<0>(), 0,
        X.template get<1>(), Y.template get<1>(), Z.template get<1>(), 0,
        X.template get<2>(), Y.template get<2>(), Z.template get<2>(), 0,
        -Dot(X, eye), -Dot(Y, eye), -Dot(Z, eye), 1
    );
}
//////////////////////////////////////////////////////////

template <typename Type>
static MV_API Mat<4u, 4u, Type> LookAt2(const Vektor<3u, Type>& eye, const Vektor<3u, Type>& target, const Vektor<3u, Type>& up)
{
    const Vektor<3u, Type> f(Normalize(target - eye));
    const Vektor<3u, Type> s(Normalize(Cross(f, up)));
    const Vektor<3u, Type> u(Cross(s, f));

    return MakeMatrix<4u, 4u, Type>(
        s.template get<0>(), s.template get<1>(), s.template get<2>(), 0,
        u.template get<0>(), u.template get<1>(), u.template get<2>(), 0,
        -f.template get<0>(), -f.template get<1>(), -f.template get<2>(), 0,
        -Dot(s, eye), -Dot(u, eye), Dot(f, eye), 1
    );
}


#if MV_EXPERIMENTAL
//Combined rotation matrix
//Roll, Pitch, Roll (ZXZ)
//Longitude, Latitude, Track direction
template <typename Type>
static MV_API Mat<3u, 3u, Type> MakeCompositeOrbitEphemerisRotationM(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    
    //mu, epsilon, ro (ZXZ)
    const Type cm = Cos(rads[0]);
    const Type sm = Sin(rads[0]);
    const Type ce = Cos(rads[1]);
    const Type se = Sin(rads[1]);
    const Type cr = Cos(rads[2]);
    const Type sr = Sin(rads[2]);
    
    return Mat<3u, 3u, Type>(
        cm*ce, sm*ce, se,
        -cm*se*sr - sm*cr, -sm*se*sr + cm*cr, ce*sr,
        -cm*se*cr + sm*sr, -sm*se*cr - cm*sr, ce*cr
    );
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Combined rotation matrix
//Roll, Pitch, Roll (ZXZ)
//to Ascending node, Orbit inclination, Argument of Latitude
template <typename Type>
static MV_API Mat<3u, 3u, Type> MakeCompositeOrbitRotationM(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    
    //omega, iota, nu (ZXZ)
    const Type co = Cos(rads[0]);
    const Type so = Sin(rads[0]);
    const Type ci = Cos(rads[1]);
    const Type si = Sin(rads[1]);
    const Type cn = Cos(rads[2]);
    const Type sn = Sin(rads[2]);
    
    return Mat<3u, 3u, Type>(
        co*cn - so*ci*sn, so*cn + co*ci*sn, si*sn,
        -co*sn - so*ci*cn, -so*sn + co*ci*cn, si*cn,
        so*si, -co*si, ci
    );
}
//////////////////////////////////////////////////////////
#endif


#if MV_EXPERIMENTAL
//Combined rotation matrix
//Pitch, yaw, roll (XYZ)
template <typename Type>
static MV_API Mat<3u, 3u, Type> MakeCompositeRotationM3(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    
    //pitch, yaw, roll
    const Type cp = Cos(rads[0]);
    const Type sp = Sin(rads[0]);
    const Type cy = Cos(rads[1]);
    const Type sy = Sin(rads[1]);
    const Type cr = Cos(rads[2]);
    const Type sr = Sin(rads[2]);
    
    return Mat<3u, 3u, Type>(
        cr*cy, sp*cy, -sy,
        cr*sy*sp - sr*cp, sr*sy*sp + cr*cp, cy*sp,
        cr*sy*cp + sr*sp, sr*sy*cp - cr*sp, cy*cp
    );
}
//////////////////////////////////////////////////////////
#endif

#if MV_EXPERIMENTAL
//Combined rotation matrix
//Pitch, yaw, roll (XYZ)
template <typename Type>
static MV_API Mat<4u, 4u, Type> MakeCompositeRotationM4(const Vektor<3u, Type>& rads)
{
    using math::Cos;
    using math::Sin;
    return (
        (
            MakeMatrix<4u, 4u, Type>(
                1, 0, 0, 0,
                0, Cos(rads[0]), -Sin(rads[0]), 0,
                0, Sin(rads[0]), Cos(rads[0]), 0,
                0, 0, 0, 1
            ) *
            MakeMatrix<4u, 4u, Type>(
                Cos(rads[1]), 0, Sin(rads[1]), 0,
                0, 1, 0, 0,
                -Sin(rads[1]), 0, Cos(rads[1]), 0,
                0, 0, 0, 1
            )
        ) *
        MakeMatrix<4u, 4u, Type>(
            Cos(rads[2]), -Sin(rads[2]), 0, 0,
            Sin(rads[2]), Cos(rads[2]), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        )
    );
}
//////////////////////////////////////////////////////////
#endif


//Vektor v rotated theta radians
//Requires standard reference frame (ijk-aligned)
template <typename Type>
static MV_API Mat<3u, 3u, Type> MakeRotationM(const Vektor<3u, Type>& v, const Type theta)
{
    const Type o = static_cast<Type>(1);
    const Type c = math::Cos(theta);
    const Type s = math::Sin(theta);
    
    return MakeMatrix<3u, 3u, Type>(
        v.template get<0>() * v.template get<0>() + (v.template get<1>() * v.template get<1>() + v.template get<2>() * v.template get<2>())*c,
        v.template get<0>() * v.template get<1>() * (o - c) - v.template get<2>() * s, 
        v.template get<0>() * v.template get<2>() * (o - c) + v.template get<1>() * s,
        v.template get<0>() * v.template get<1>() * (o - c) + v.template get<2>() * s, 
        v.template get<1>() * v.template get<1>() + (v.template get<2>() * v.template get<2>() + v.template get<0>() * v.template get<0>())*c, 
        v.template get<1>() * v.template get<2>() * (o - c) - v.template get<0>() * s,
        v.template get<2>() * v.template get<0>() * (o - c) - v.template get<1>() * s, 
        v.template get<1>() * v.template get<2>() * (o - c) + v.template get<0>() * s, 
        v.template get<2>() * v.template get<2>() + (v.template get<0>() * v.template get<0>() + v.template get<1>() * v.template get<1>())*c
    );
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type>
static MV_API Mat<Size + 1u, Size + 1, Type> MakeScaling(const Vektor<Size, Type>& scaling)
{
    static_assert(Size < 254u, "MakeScaling: Tried to create too large scaling matrix");
    std::array<Type, (Size + 1u) * (Size + 1u)> arr{};
    for (UInt8 i = 0u; i < (Size + 1u); ++i)
    {
        for (UInt8 j = 0u; j < (Size + 1u); ++j)
        {
            arr[(i * (Size + 1u)) + j] = ((i == j) && (i < Size) && (j < Size) ? scaling[i] : static_cast<Type>(0));
        }
    }
    arr[((Size + 1u) * (Size + 1u))  - 1u] = static_cast<Type>(1);
    return Mat<Size + 1u, Size + 1u, Type>(arr);
}
//////////////////////////////////////////////////////////

template <typename Type>
static MV_API Mat<4u, 4u, Type> MakeScalingViaPoint(const Vektor<3u, Type>& s, const Vektor<3u, Type>& p)
{
    return MakeMatrix<4u, 4u, Type>(
        s.template get<0>(), 0, 0, p.template get<0>() * (static_cast<Type>(1) - s.template get<0>()),
        0, s.template get<1>(), 0, p.template get<1>() * (static_cast<Type>(1) - s.template get<1>()),
        0, 0, s.template get<2>(), p.template get<2>() * (static_cast<Type>(1) - s.template get<2>()),
        0, 0, 0, 1
    );
}
//////////////////////////////////////////////////////////


template <UInt16 Size, typename Type>
static MV_API Mat<Size + 1u, Size + 1u, Type> MakeTranslation(const Vektor<Size, Type>& translation)
{
    static_assert(Size < 254u, "MakeTranslation: Tried to create too large translation matrix");
    std::array<Type, (Size + 1u) * (Size + 1u)> arr{};
    for (UInt8 i = 0u; i < (Size + 1u); ++i)
    {
        for (UInt8 j = 0u; j < (Size + 1u); ++j)
        {
            arr[(i * (Size + 1u)) + j] = static_cast<Type>((i == j) ? 1 : 0);
        }
        //if (i < Size) arr[(i * (Size + 1u)) + Size] = translation[i];
    }
    for (UInt16 i = 0u; i < Size; ++i)
    {
        arr[(Size * (Size + 1u)) + i] = translation[i];
    }
    return Mat<Size + 1u, Size + 1u, Type>(arr);
}
//////////////////////////////////////////////////////////


//Multiply matrix and vektor
template <UInt8 Rows, UInt8 Cols, typename Type,  UInt16 Size>
static MV_API Mat<Rows, 1u, Type> operator*(const Mat<Rows, Cols, Type>& m, const Vektor<Size, Type>& v)
{
    static_assert(Cols == Size, "Matrix size mismatch");
    return m * ToMatrix<Rows, 1u>(v);
}
//////////////////////////////////////////////////////////


//Multiply vektor and matrix
template <UInt8 Rows, UInt8 Cols, typename Type, UInt16 Size>
static MV_API Mat<1u, Cols, Type> operator*(const Vektor<Size, Type>& v, const Mat<Rows, Cols, Type>& m)
{
    static_assert(Rows == Size, "Matrix size mismatch");
    return ToMatrix<1u, Rows>(v) * m;
}
//////////////////////////////////////////////////////////


//v = axis
template <typename Type>
static MV_API Mat<4u, 4u, Type> RotateAroundAxis(const Vektor<3u, Type>& v, const Type rad)
{
    const Type s = math::Sin(rad);
    const Type c = math::Cos(rad);
    const Type K = static_cast<Type>(1) - c;
    return MakeMatrix<4u, 4u, Type>(
        (v.template get<0>() * v.template get<0>()) * K + c,
        v.template get<0>() * v.template get<1>() * K - v.template get<2>() * s,
        v.template get<0>() * v.template get<2>() * K + v.template get<1>() * s,
        v.template get<0>() * v.template get<1>() * K + v.template get<2>() * s,
        (v.template get<1>() * v.template get<1>()) * K + c,
        v.template get<1>() * v.template get<2>() * K - v.template get<0>() * s,
        v.template get<0>() * v.template get<2>() * K - v.template get<1>() * s,
        v.template get<1>() * v.template get<2>() * K + v.template get<0>() * s,
        (v.template get<2>() * v.template get<2>()) * K + c
    );
}


//Vektor to skew-symmetric matrix (for Mat-Vek Cross)
template <typename Type>
static MV_API Mat<3u, 3u, Type> Tilde(const Vektor<3u, Type>& v)
{
    return MakeMatrix<3u, 3u, Type>(
        0, -v.template get<2>(), v.template get<1>(),
        v.template get<2>(), 0, -v.template get<0>(),
        -v.template get<1>(), v.template get<0>(), 0
    );
}
//////////////////////////////////////////////////////////


#if MV_EXPERIMENTAL
template <typename Type>
static MV_API Vektor<4u, Type> TransformDirection(const Mat<4u, 4u, Type>& m, const Vektor<4u, Type>& v)
{
    return ToVektor(m * v);
}
//////////////////////////////////////////////////////////
#endif


#if 0
//Search for pattern
//Returns pattern starting coordinates if found, out of bound coordinates otherwise
//Exactness: 0-ignored, 1-exact requirement, 2+ required somewhere, use EV
template <
UInt8 TarRows, UInt8 TarCols,
UInt8 PatRows, UInt8 PatCols,
typename Type, UInt8 EVSize = 0u>
static Mat<1u, 2u, UInt8> SearchForPattern(
    const Mat<TarRows, TarCols, Type>& target,
    const Mat<PatRows, PatCols, Type>& pattern,
    const Mat<PatRows, PatCols, UInt8>* exactness = nullptr,
    const Vektor<EVSize, UInt8>* EVCounts = nullptr,
    const Vektor<EVSize, Type>* EVValues = nullptr
)
{
    static_assert(PatCols > 0u, "Too small pattern to search for");
    static_assert(PatRows > 0u, "Too small pattern to search for");
    static_assert(PatRows < TarRows, "Pattern is larger than searchable area");
    static_assert(PatCols < TarCols, "Pattern is larger than searchable area");
    
    const UInt8 lastMatch = (PatRows * PatCols) - 1u;
    bool lastWasFalse = true;
    
    Mat<PatRows, PatCols, bool> match;
    match.Fill(false);
    
    Vektor<EVSize, UInt8> evmatchcounter;
    evmatchcounter.Fill(0u);
    
    
    Mat<PatRows, PatCols, UInt8> exact;
    if (exactness)
    exact = *exactness;
    else
    exact.Fill(1u);
    
    
    for (UInt8 tarY = 0u; tarY < TarRows; ++tarY)
    {
        for (UInt8 tarX = 0u; tarX < TarCols; ++tarX)
        {
            for (UInt8 patY = 0u; patY < PatRows; ++patY)
            {
                for (UInt8 patX = 0u; patX < PatCols; ++patX)
                {
                    if (tarY + patY >= TarRows || tarX + patX >= TarCols)
                    {
                        break;
                    }
                    else if ((target.at(tarY + patY, tarX + patX) == pattern.at(patY, patX) && exact.at(patY, patX) == 1u) || (exact.at(patY, patX) == 0u))
                    {
                        match.at(patY, patX) = true;
                        lastWasFalse = false;
                    }
                    else
                    {
                        for (UInt8 ev = 0u; ev < EVSize; ++ev)
                        {
                            if (target.at(tarY + patY, tarX + patX) == (*EVValues)[ev])
                            {
                                ++evmatchcounter[ev];
                            }
                        }
                        match.at(patY, patX) = true;
                        lastWasFalse = false;
                        if (lastWasFalse)
                        {
                            break;
                        }
                    }
                }
                if (lastWasFalse)
                {
                    break;
                }
                else
                {
                    lastWasFalse = true;
                }
            }
            if (match[lastMatch] == true)
            {
                if (std::all_of(match.data().begin() + 1u, match.data().end(), [](bool b) {return b == true; }))
                {
                    bool allTrue = true;
                    for (UInt8 m = 0u; m < EVSize; ++m)
                    {
                        if ((*EVCounts)[m] > evmatchcounter[m])
                        {
                            evmatchcounter.Fill(0u);
                            allTrue = false;
                            break;
                        }
                    }
                    if (allTrue)
                    {
                        return Mat<1u, 2u, UInt8>(tarY, tarX);
                    }
                }
            }
            else
            {
                match.Fill(false);
                lastWasFalse = true;
            }
        }
    }
    
    //Pattern not found
    return Mat<1u, 2u, UInt8>(TarRows, TarCols);
}
#endif // if 0 Pattern


#endif // MV_MVF_HPP
