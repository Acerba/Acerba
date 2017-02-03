#pragma once

#include <Ace\Vector4.h>
#include <Ace\IntTypes.h>

namespace ace
{
	namespace math
	{

		class Matrix4
		{
		public:

			union
			{
				struct
				{
					Vector4 rows[4];
				};

				float array[16];

				// Rows, Columns
				float data[4][4];

			};

			Matrix4();
			Matrix4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4);

			float Determinant() const;
			Matrix4 Transpose() const;
			Matrix4 Inverse() const;
			Matrix4 Cofactor() const;
			Matrix4 Adjunct() const;
			Matrix4 operator*(const Matrix4& m) const;
			Vector4 operator*(const Vector4& o) const;
			Matrix4 operator*(float scalar) const;

			static Matrix4 Scale(float x, float y, float z);
			static Matrix4 Translation(const Vector4& t);
			static Matrix4 RotationX(float a);
			static Matrix4 RotationY(float a);
			static Matrix4 RotationZ(float a);


			Vector4& operator[](UInt8 index)
			{
				return rows[index];
			}

			Vector4 operator[](UInt8 index)const
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