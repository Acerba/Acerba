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

#ifndef MV_QUATERNION_HPP
#define MV_QUATERNION_HPP

#include "../utilities/Utilities.hpp"

namespace mv
{
    
    template <typename Type = MV_TYPE>
    class Quat
    {
        
    public:
        
        union
        {
            struct
            {
                Type _s;
                #ifdef MV_VEKTOR_HPP
                Vektor<3u, Type> _v;
                #else
                std::array<Type, 3u> _v;
                #endif
            };
            
            struct
            {
                Type S;
                
                Type X;
                Type Y;
                Type Z;
            };
        };
        
        
        MV_API Quat() :
        S(static_cast<Type>(1)),
        X(static_cast<Type>(0)),
        Y(static_cast<Type>(0)),
        Z(static_cast<Type>(0))
        {
            
        }
        //////////////////////////////////////////////////////////
        
        
        //Ctor
        MV_API Quat(Type s, Type x, Type y, Type z) :
        S(s),
        X(x),
        Y(y),
        Z(z)
        {
            
        }
        //////////////////////////////////////////////////////////
        
        
        //Ctor
        MV_API Quat(Type s, const std::array<Type, 3u>& v) :
        _s(s),
        _v(v)
        {
            
        }
        //////////////////////////////////////////////////////////
        
        
        //Ctor (pure quaternion)
        MV_API Quat(const std::array<Type, 3u>& v) :
        _s(static_cast<Type>(0)),
        _v(v)
        {
            
        }
        //////////////////////////////////////////////////////////
        
        
        template <typename T>
        MV_API operator Quat<T>()
        {
            return Quat<T>(
                static_cast<T>(S),
                static_cast<T>(X),
                static_cast<T>(Y),
                static_cast<T>(Z)
            );
        }
        //////////////////////////////////////////////////////////
        
        
        MV_API Type length() const
        {
            return math::Sqrt(lengthSquared());
        }
        //////////////////////////////////////////////////////////
        
        
        MV_API Type lengthSquared() const
        {
            return S*S + X*X + Y*Y + Z*Z;
        }
        //////////////////////////////////////////////////////////
        
        
        MV_API void normalize()
        {
            Type l = length();
            if(math::Epsilon(l))
            {
                MV_ASSERT(false, "Quaternion division by zero");
                l = static_cast<Type>(detail::MV_BOUNDARY);
            }
            l = static_cast<Type>(1) / l;
            S *= l;
            X *= l;
            Y *= l;
            Z *= l;
        }
        //////////////////////////////////////////////////////////
        
        
        //Increment
        MV_API Quat& operator+=(const Quat& rhs)
        {
            S += rhs.S;
            X += rhs.X;
            Y += rhs.Y;
            Z += rhs.Z;
            return *this;
        }
        MV_API friend Quat operator+(Quat lhs, const Quat& rhs)
        {
            lhs += rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
        
        //Substract
        MV_API Quat& operator-=(const Quat& rhs)
        {
            S -= rhs.S;
            X -= rhs.X;
            Y -= rhs.Y;
            Z -= rhs.Z;
            return *this;
        }
        MV_API friend Quat operator-(Quat lhs, const Quat& rhs)
        {
            lhs -= rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
        
        // Scale
        MV_API Quat& operator*=(const Type& rhs)
        {
            S *= rhs;
            X *= rhs;
            Y *= rhs;
            Z *= rhs;
            return *this;
        }
        MV_API friend Quat operator*(Quat lhs, const Type& rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
        
        //Multiply
        MV_API Quat& operator*=(const Quat& rhs)
        {
            #ifdef MV_VEKTOR_HPP
            const Type scalar = (S * rhs.S) - Dot(_v, rhs._v);
            _v = Scale(rhs._v, _s) + Scale(_v, rhs._s) + Cross(_v, rhs._v);
            _s = scalar;
            #else
            Type scalar = static_cast<Type>(0);
            std::array<Type, 3u> cp{
                ((_v[1] * rhs._v[2]) - (_v[2] * rhs._v[1])),
                -((_v[0] * rhs._v[2]) - (_v[2] * rhs._v[0])),
                ((_v[0] * rhs._v[1]) - (_v[1] * rhs._v[0]))
            };
            for (UInt8 i = 0u; i < 3u; ++i)
            {
                scalar += _v[i] + rhs._v[i];
                _v[i] = ((rhs._v[i] * _s) + (_v[i] * rhs._s) + cp[i]);
            }
            _s = (S * rhs.S) - scalar;
            #endif
            return *this;
        }
        MV_API friend Quat operator*(Quat lhs, const Quat& rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
    }; //quat
    
    #include "../functions/QuaternionFunctions.hpp"
    
    #ifdef MV_MATRIX_HPP
    #include "../functions/MatrixQuaternionFunctions.hpp"
    #endif
    #ifdef MV_VEKTOR_HPP
    #include "../functions/QuaternionVektorFunctions.hpp"
    #endif
    
} // mv

#endif // MV_QUATERNION_HPP
