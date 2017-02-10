#pragma once

#include <Ace\Vector2.h>
#include <Ace\IntTypes.h>


namespace ace
{
	namespace math
	{

		class Matrix2
		{
		public:

			union
			{
				struct
				{
					Vector2 rows[2];
				};

				float array[4];

				// Rows, Columns
				float data[2][2];

			};

			Matrix2();
			Matrix2(Vector2 v1, Vector2 v2);

			float Determinant() const;
			Matrix2 Transpose() const;
			Matrix2 Inverse() const;
			Matrix2 Cofactor() const;
			Matrix2 Adjunct() const;
			Matrix2 operator*(const Matrix2& m) const;
			Vector2 operator*(const Vector2& o) const;
			Matrix2 operator*(float scalar) const;




			static Matrix2 Scale(float x, float y);
			static Matrix2 Rotation(float a);
			


			Vector2& operator[](UInt8 index)
			{
				return rows[index];
			}

			Vector2 operator[](UInt8 index)const
			{
				return rows[index];
			}

			float& operator()(UInt8 r, UInt8 c)
			{
				return data[r][c];
			}

			float operator()(UInt8 r, UInt8 c)const
			{
				return data[r][c];
			}


		};

	}
}