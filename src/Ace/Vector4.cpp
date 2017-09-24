#include <Ace/Vector4.h>
#include <Ace/Math.h>

namespace ace
{
    namespace math
    {
        Vector4::Vector4(float x, float y, float z, float w) : array{ x, y, z, w } { }

        float Vector4::Length() const
        {
            return Sqrt(LengthSquared());
        }

        float Vector4::LengthSquared() const
        {
            return x*x + y*y + z*z + w*w;
        }

        Vector4 Vector4::Normalize() const
        {
            const float temp = Length();
            const float l = temp > epsilon ? temp : epsilon;
            return { x / l, y / l, z / l, w / l };
        }

        Vector4 Vector4::operator+(const Vector4& v) const
        {
            return { x + v.x, y + v.y, z + v.z, w + v.w };
        }

        Vector4& Vector4::operator+=(const Vector4& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }

        Vector4 Vector4::operator-(const Vector4& v) const
        {
            return { x - v.x, y - v.y, z - v.z, w - v.w };
        }

        Vector4& Vector4::operator-=(const Vector4& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }

        Vector4 Vector4::operator*(float scalar) const
        {
            return { x * scalar, y *scalar, z *scalar, w *scalar };
        }

        Vector4& Vector4::operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        Vector4 Vector4::operator/(float scalar) const
        {
            return { x / scalar, y / scalar, z / scalar, w / scalar };
        }

        Vector4& Vector4::operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            z /= scalar;
            return *this;
        }

        float Vector4::Dot(const Vector4& v) const
        {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        float& Vector4::operator[](UInt8 index)
        {
            return array[index];
        }
        
        float Vector4::operator[](UInt8 index)const
        {
            return array[index];
        }
    }
}