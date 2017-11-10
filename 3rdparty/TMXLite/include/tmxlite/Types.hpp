/*********************************************************************
Matt Marchant 2016
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#ifndef TMXLITE_TYPES_HPP_
#define TMXLITE_TYPES_HPP_

#include <cstdint>

namespace tmx
{
    typedef char* (*FileCallback)(const char* path);
    extern FileCallback fileCallback;
    extern bool editPaths;

    /*!
    \brief Two dimensional vector used to store points and positions
    */
    template <class T>
    struct Vector2 final
    {
        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) :x(x), y(y) {}
        T x, y;
    };

    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int>;
    using Vector2u = Vector2<unsigned>;

    template <typename T>
    Vector2<T> operator + (const Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T>& operator += (Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T> operator - (const Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T>& operator -= (Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T> operator * (const Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T>& operator *= (Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T> operator * (const Vector2<T>& l, T r);

    template <typename T>
    Vector2<T>& operator *= (Vector2<T>& l, T r);

    template <typename T>
    Vector2<T> operator / (const Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T>& operator /= (Vector2<T>& l, const Vector2<T>& r);

    template <typename T>
    Vector2<T> operator / (const Vector2<T>& l, T r);

    template <typename T>
    Vector2<T>& operator /= (Vector2<T>& l, T r);

#include "Types.inl"

    /*!
    \brief Describes a rectangular area, such as an AABB (axis aligned bounding box)
    */
    template <class T>
    struct Rectangle final
    {
        Rectangle() : left(0), top(0), width(0), height(0) {}
        Rectangle(T l, T t, T w, T h) : left(l), top(t), width(w), height(h) {}
        Rectangle(Vector2<T> position, Vector2<T> size) : left(position.x), top(position.y), width(size.x), height(size.y) {}
        T left, top, width, height;
    };

    using FloatRect = Rectangle<float>;
    using IntRect = Rectangle<int>;

    /*!
    \brief Contains the red, green, blue and alpha values of a colour
    in the range 0 - 255.
    */
    struct Colour final
    {
        Colour(std::uint8_t red = 0, std::uint8_t green = 0, std::uint8_t blue = 0, std::uint8_t alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}
        std::uint8_t r, g, b, a;
    };
}

#endif //TMXLITE_TYPES_HPP_