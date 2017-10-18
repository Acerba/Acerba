#include <Ace/Matrix2.h>
#include <Ace/Math.h>

namespace ace
{
    namespace math
    {

        Matrix2::Matrix2() : rows() {}

        Matrix2::Matrix2(const Vector2& v1, const Vector2& v2)
        {
            rows[0] = v1;
            rows[1] = v2;
        }

        Matrix2::Matrix2(float a00, float a01, float a10, float a11)
        {
            data[0][0] = a00;
            data[0][1] = a01;
            data[1][0] = a10;
            data[1][1] = a11;
        }

        Matrix2::Matrix2(const float identity) : rows()
        {
            data[0][0] = data[1][1] = identity;
        }


        float Matrix2::Determinant() const
        {
            return (data[0][1] * data[1][0]) - (data[1][1] * data[0][0]);
        }

        Matrix2 Matrix2::Transpose() const
        {
            return Matrix2(
                data[0][0], data[1][0],
                data[0][1], data[1][1]
            );
        }

        Matrix2 Matrix2::Inverse() const
        {
            return Adjunct() * (1.f / Determinant());
        }

        Matrix2 Matrix2::Cofactor() const
        {
            return Matrix2(
                -data[1][1], data[1][0],
                data[0][1], -data[0][0]
            );
        }

        Matrix2 Matrix2::Adjunct() const
        {
            return Cofactor().Transpose();
        }

        Matrix2& Matrix2::operator*=(const Matrix2& m)
        {
            rows[0] = Vector2(rows[0].x*m(0, 0) + rows[0].y*m(1, 0), rows[0].x*m(0, 1) + rows[0].y*m(1, 1));
            rows[1] = Vector2(rows[1].x*m(0, 0) + rows[1].y*m(1, 0), rows[1].x*m(0, 1) + rows[1].y*m(1, 1));
            return *this;
        }
        Matrix2 operator*(Matrix2 lhs, const Matrix2& rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        /*
        Vector2 Matrix2::operator*(const Vector2& o) const
        {
            return Vector2(
                rows[0].x * o.x + rows[0].y * o.y,
                rows[1].x * o.x + rows[1].y * o.y
            );
        }
        */

        Matrix2& Matrix2::operator*=(float s)
        {
            for (auto& itr : this->array)
                itr *= s;
            return *this;
        }
        Matrix2 operator*(Matrix2 lhs, float s)
        {
            lhs *= s;
            return lhs;
        }

        Matrix2 Matrix2::Identity()
        {
            return Matrix2(1.f);
        }

        Matrix2 Matrix2::Scale(float x, float y)
        {
            return Matrix2(
                Vector2(x, 0.f),
                Vector2(0.f, y)
            );
        }


        Matrix2 Matrix2::Rotation(float a)
        {
            a = Rad(a);
            return Matrix2(
                Vector2(Cos(a), -Sin(a)),
                Vector2(Sin(a), Cos(a))
            );
        }
    }
}