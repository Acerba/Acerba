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

#ifndef MV_UTILITIES_HPP
#define MV_UTILITIES_HPP

#include "Types.hpp"

#include <array>
#include <cmath>
//#include <cstdio>
//#include <cstdlib>
#include <type_traits>

#if MV_DEBUG
    #include <iostream>
    #include <string>
    #include <sstream>
#endif

#if MV_CONSTEXPR
    #define MV_API constexpr
#else
    #define MV_API  
#endif

#define MV_DISABLE_CLASS(name) \
    name() = delete; \
    ~name() = delete; \
    name(name&&) = delete; \
    name(const name&) = delete; \
    name& operator=(name&&) = delete; \
    name& operator=(const name&) = delete;

namespace mv
{
#if MV_DEBUG
    namespace detail
    {
        template <typename MsgType>
        static std::string _msgImpl(const MsgType& m)
        {
            std::ostringstream ss;
            ss << m;
            return ss.str();
        }
        
        template <typename MsgType, typename ... MsgTypes>
        static std::string _msgImpl(const MsgType& m, const MsgTypes& ... ms)
        {
            return _msgImpl(m) + _msgImpl(ms...);
        }
    } //detail
    
    template <typename ... MsgTypes>
    static void _msg(const MsgTypes& ... ms)
    {
        std::cout << detail::_msgImpl(ms...);
    }
    
    namespace detail
    {
        static void assertion(const bool expression, const char* message)
        {
            if (!expression)
            {
                _msg(message);
                #if _WIN32
                    __debugbreak();
                #endif
                std::exit(EXIT_FAILURE);
            }
        }
    } //detail
    
    #define MV_ASSERT(expression, message) ::mv::detail::assertion(expression, message)
#else // MV_DEBUG
    #define MV_ASSERT(expression, message)
#endif // MV_DEBUG
    
    
    namespace detail
    {
        
        //Default type for matrices and vectors
        #if defined (MV_USE_LONG_DOUBLE)
        #define MV_TYPE long double
        #elif defined (MV_USE_DOUBLE)
        #define MV_TYPE double
        #else
        #define MV_TYPE float
        #endif
        
        #if defined (MV_BOUNDARY_STRICT)
        static const MV_TYPE MV_BOUNDARY = static_cast<MV_TYPE>(0.0001);
        #elif defined (MV_BOUNDARY_LOOSE)
        static const MV_TYPE MV_BOUNDARY = static_cast<MV_TYPE>(0.01);
        #else
        static const MV_TYPE MV_BOUNDARY = static_cast<MV_TYPE>(0.001);
        #endif
        
        //How strict constructors should be
        #if defined (MV_MAT_STRICT)
        template <typename T1, typename T2>
        using MV_IsAllowed = std::is_same<T1, T2>;
        #else
        template <typename T1, typename T2>
        using MV_IsAllowed = std::is_convertible<T1, T2>;
        #endif
        
        
        //Base
        template <typename, typename...>
        struct AcceptedType{};
        //Last arg
        template <typename Type, typename T>
        struct AcceptedType<Type, T>
        { enum : bool { value = MV_IsAllowed<T, Type>::value }; };
        //Check constructor arguments recursively
        template <typename Type, typename T, typename ... Rest>
        struct AcceptedType<Type, T, Rest...>
        {
            enum : bool { value = MV_IsAllowed<T, Type>::value };
            static_assert(value && AcceptedType<Type, Rest...>::value, "One or more generic matrix constructor parameter is incompatible with desired type.");
        };
        
    } //detail
    
    
    //For accessing matrix via []-operator with two arguments
    struct Index
    {
        UInt8 Row;
        UInt8 Col;
        Index(const UInt8 row, const UInt8 col) : Row(row), Col(col) {}
    };
    
    enum class AXIS : Int8
    {
        Invalid = -1,
        X = 0, Pitch = 0,
        Y = 1, Yaw = 1,
        Z = 2, Roll = 2,
    };
    
    
    //Math
    namespace math
    {
        MV_API const double MV_PI = 3.14159265358979;
        
        template <typename Type>
        inline static Type Abs(const Type val)
        {
            return static_cast<Type>(std::abs(val));
        }
        template<typename Type>
        inline static Type ACos(const Type rad)
        {
            return static_cast<Type>(std::acos(rad));
        }
        template<typename Type>
        inline static Type ASin(const Type rad)
        {
            return static_cast<Type>(std::asin(rad));
        }
        template<typename Type>
        inline static Type ATan(const Type rad)
        {
            return static_cast<Type>(std::atan(rad));
        }
        template<typename Type>
        inline static Type ATan2(const Type y, const Type x)
        {
            return static_cast<Type>(std::atan2(y, x));
        }
        template<typename Type>
        inline static Type Cos(const Type rad)
        {
            return static_cast<Type>(std::cos(rad));
        }
        template <typename Type = MV_TYPE>
        static Type Pi()
        {
            return static_cast<Type>(MV_PI);
        }
        template<typename Type, typename T>
        inline static Type Pow(const Type val, const T exp)
        {
            return static_cast<Type>(std::pow(val, exp));
        }
        template<typename Type>
        inline static Type Sin(const Type rad)
        {
            return static_cast<Type>(std::sin(rad));
        }
        template<typename Type>
        inline static Type Sqrt(const Type val)
        {
            return static_cast<Type>(std::sqrt(val));
        }
        template<typename Type>
        inline static Type Tan(const Type rad)
        {
            return static_cast<Type>(std::tan(rad));
        }
        
        //Returns clamped value
        template <typename Type>
        static Type Clamp(const Type val, const Type min, const Type max)
        {
            return val < min ? min : max < val ? max : val;
        }
        //Returns true if val is close to zero
        template <typename Type>
        static bool Epsilon(const Type val)
        {
            return
            -detail::MV_BOUNDARY < static_cast<MV_TYPE>(val) &&
            static_cast<MV_TYPE>(val) < detail::MV_BOUNDARY;
        }
        //Returns smallest value
        template <typename Type>
        static Type GetLargest(const Type a, const Type b, const Type c)
        {
            return (b < a) ? ((c < a) ? a : c) : ((c < b) ? b : c);
        }
        //Returns index of smallest element
        template <typename Type>
        static UInt8 GetLargest(const Type(&arr)[3u])
        {
            const Type max = GetLargest(arr[0], arr[1], arr[2]);
            return arr[0] == max ? 0u : arr[1] == max ? 1u : 2u;
        }
        //Returns smallest value
        template <typename Type>
        static Type GetSmallest(const Type a, const Type b, const Type c)
        {
            return (a < b) ? ((c < a) ? c : a) : ((c < b) ? c : b);
        }
        //Returns index of the smallest element
        template <typename Type>
        static UInt8 GetSmallest(const Type(&arr)[3u])
        {
            const Type max = GetSmallest(arr[0], arr[1], arr[2]);
            return arr[0] == max ? 0u : arr[1] == max ? 1u : 2u;
        }
        //Runge-Kutta4
        template <typename Type, typename Function>
        static Type RungeKutta4(const Type x, const Type y, const Type dx, Function f)
        {
            const Type t = static_cast<Type>(2);
            const Type k0 = f(x, y);
            const Type k1 = f(x + (dx / t), y + (k0 / t) * dx);
            const Type k2 = f(x + (dx / t), y + (k1 / t) * dx);
            const Type k3 = f(x + dx, y + k2 * dx);
            return y + ((dx * (k0 + t*k1 + t*k2 + k3)) / static_cast<Type>(6));
        }
        //////////////////////////////////////////////////////////
        template <typename Type>
        static MV_API Type ToDeg(const Type rad)
        {
            return static_cast<Type>(static_cast<MV_TYPE>(rad)*static_cast<MV_TYPE>(180.0) / static_cast<MV_TYPE>(math::MV_PI));
        }
        template <typename Type>
        static MV_API Type ToRad(const Type deg)
        {
            return static_cast<Type>(static_cast<MV_TYPE>(deg)*static_cast<MV_TYPE>(math::MV_PI) / static_cast<MV_TYPE>(180.0));
        }
        //////////////////////////////////////////////////////////
        
    } // math
    
    using math::ToDeg;
    using math::ToRad;
    
} // mv

#endif // MV_UTILITIES_HPP
