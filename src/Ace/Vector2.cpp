#include <Ace/Vector2.h>
#include <Ace/Matrix2.h>
#include <Ace/Math.h>

namespace ace
{
    namespace math
    {
        Vector2::Vector2(float x, float y) : x(x), y(y) {}

        float Vector2::Length() const
        {
            return Sqrt(LengthSquared());
        }

        float Vector2::LengthSquared() const
        {
            return x*x + y*y;
        }

        Vector2 Vector2::Normalize() const
        {
            const float temp = Length();
            const float l = temp > epsilon ? temp : epsilon;
            return { x / l, y / l };
        }

        Vector2 Vector2::operator+(const Vector2& v) const
        {
            return { x + v.x, y + v.y };
        }

        Vector2& Vector2::operator+=(const Vector2& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vector2 Vector2::operator-(const Vector2& v) const
        {
            return { x - v.x, y - v.y };
        }

        Vector2& Vector2::operator-=(const Vector2& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Vector2 Vector2::operator*(float scalar) const
        {
            return { x * scalar, y *scalar };
        }

        Vector2& Vector2::operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2& Vector2::operator*=(const Matrix2& m)
        {
            x = (m(0, 0) * x) + (m(0, 1) * y);
            y = (m(1, 0) * x) + (m(1, 1) * y);
            return *this;
        }
        Vector2 operator*(Vector2 lhs, const Matrix2& rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        Vector2 operator*(const Matrix2& lhs, Vector2 rhs)
        {
            rhs *= lhs;
            return rhs;
        }

        Vector2 Vector2::operator/(float scalar) const
        {
            return { x / scalar, y / scalar };
        }

        Vector2& Vector2::operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        float Vector2::Cross(const Vector2& v) const
        {
            return x * v.y - y * v.x;
        }

        float Vector2::Cross(const Vector2& a, const Vector2& b)
        {
            return a.Cross(b);
        }

        float Vector2::Dot(const Vector2& v) const
        {
            return x * v.x + y * v.y;
        }

        float Vector2::Dot(const Vector2& a, const Vector2& b)
        {
            return a.Dot(b);
        }

        Vector2 Vector2::Invert() const
        {
            return { -x, -y };
        }
    }
}