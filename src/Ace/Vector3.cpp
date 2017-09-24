#include <Ace/Vector3.h>
#include <Ace/Math.h>

namespace ace
{
    namespace math
    {
        Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        float Vector3::Length() const
        {
            return Sqrt(LengthSquared());
        }

        float Vector3::LengthSquared() const
        {
            return x*x + y*y + z*z;
        }

        Vector3 Vector3::Normalize() const
        {
            const float temp = Length();
            const float l = temp > epsilon ? temp : epsilon;
            return { x / l, y / l, z / l };
        }

        Vector3 Vector3::operator+(const Vector3& v) const
        {
            return { x + v.x, y + v.y, z + v.z };
        }

        Vector3& Vector3::operator+=(const Vector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3 Vector3::operator-(const Vector3& v) const
        {
            return{ x - v.x, y - v.y, z - v.z};
        }

        Vector3& Vector3::operator-=(const Vector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3 Vector3::operator*(float scalar) const
        {
            return { x * scalar, y * scalar, z * scalar };
        }

        Vector3& Vector3::operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3 Vector3::operator/(float scalar) const
        {
            return { x / scalar, y / scalar, z / scalar };
        }

        Vector3& Vector3::operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        inline Vector3::operator Vector2()const
        {
            return Vector2(x, y);
        }

        Vector3 Vector3::Cross(const Vector3& v) const
        {
            return {
                y * v.z - z * v.y,
                z * v.x - x * v.z, 
                x * v.y - y * v.x
            };
        }

        float Vector3::Dot(const Vector3& v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }

        float& Vector3::operator[](UInt8 index)
        {
            return array[index];
        }

        float Vector3::operator[](UInt8 index) const
        {
            return array[index];
        }
    }
}