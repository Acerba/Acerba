#pragma once

#include <Ace/Vector3.h>
#include <Ace/IntTypes.h>


namespace ace
{
	namespace math
	{

		class Matrix3
		{
		public:

			union
			{
				struct
				{
					Vector3 rows[3];
				};

				float array[9];
				
				// Rows, Columns
				float data[3][3];

			};

			Matrix3();
			Matrix3(float identity);
			Matrix3(Vector3 v1, Vector3 v2, Vector3 v3);

			float Determinant() const;
			Matrix3 Transpose() const;
			Matrix3 Inverse() const;
			Matrix3 Cofactor() const;
			Matrix3 Adjunct() const;
			Matrix3 operator*(const Matrix3& m) const;
			Vector3 operator*(const Vector3& o) const;
			Matrix3 operator*(float scalar) const;



			static Matrix3 Identity();
			static Matrix3 Scale(float x, float y, float z);
			static Matrix3 Translation(const Vector3& t);
			static Matrix3 RotationX(float a);
			static Matrix3 RotationY(float a);
			static Matrix3 RotationZ(float a);
			


			Vector3& operator[](UInt8 index)
			{
				return rows[index];
			}

			Vector3 operator[](UInt8 index)const
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