#include <Ace/Matrix2.h>

namespace ace
{
	namespace math
	{

		Matrix2::Matrix2() : rows() {}

		Matrix2::Matrix2(Vector2 v1, Vector2 v2)
		{
			rows[0] = v1;
			rows[1] = v2;
		}

		Matrix2::Matrix2(const float identity) : rows()
		{
			data[0][0] = data[1][1] = identity;
		}

		float Matrix2::Determinant() const
		{
			Matrix2 m = *this;

			return
				m(0, 1)*m(1, 0) - m(1, 1)*m(0, 0);
		}

		Matrix2 Matrix2::Transpose() const
		{
			Matrix2 t;

			t(0, 0) = (*this)(0, 0);
			t(1, 1) = (*this)(1, 1);
			t(0, 1) = (*this)(1, 0);
			t(1, 0) = (*this)(0, 1);
		
			return t;

		}

		Matrix2 Matrix2::Inverse() const
		{
			return Adjunct() * (1.f / Determinant());
		}

		Matrix2 Matrix2::Cofactor() const
		{
			Matrix2 m;

			m(0, 0) = -data[1][1];
			m(1, 1) = -data[0][0];
			m(1, 0) = data[0][1];
			m(0, 1) = data[1][0];

			return m;
		}

		Matrix2 Matrix2::Adjunct() const
		{
			Matrix2 adj = Cofactor().Transpose();
			return adj;
		}

		Matrix2 Matrix2::operator*(const Matrix2& m) const
		{
			return Matrix2(
				Vector2(rows[0].x*m(0, 0) + rows[0].y*m(1, 0), rows[0].x*m(0, 1) + rows[0].y*m(1, 1)),
				Vector2(rows[1].x*m(0, 0) + rows[1].y*m(1, 0), rows[1].x*m(0, 1) + rows[1].y*m(1, 1)));
		}

		Vector2 Matrix2::operator*(const Vector2& o) const
		{
			return Vector2(
				rows[0].x * o.x + rows[0].y * o.y,
				rows[1].x * o.x + rows[1].y * o.y);
		}

		Matrix2 Matrix2::operator*(float s)const
		{
			Matrix2 mat = *this;

			for (int i = 0; i < 4; ++i)
			{
				mat.array[i] *= s;
			}

			return mat;
		}

		Matrix2 Matrix2::Identity()
		{
			return Matrix2(1.f);
		}

		Matrix2 Matrix2::Scale(float x, float y)
		{
			Vector2 r1(x, 0.f);
			Vector2 r2(0.f, y);
			

			return Matrix2(r1, r2);
		}


		Matrix2 Matrix2::Rotation(float a)
		{
			a = Rad(a);
			Vector2 r1(Cos(a), -Sin(a));
			Vector2 r2(Sin(a), Cos(a));

			return Matrix2(r1, r2);
		}
	}
}