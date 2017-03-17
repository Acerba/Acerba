#pragma once

#include <Ace/Vector3.h>
#include <Ace/Vector2.h>
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
			
			/**
			@return Determinant
			*/
			float Determinant() const;
			/**
			@return Transpose matrix
			*/
			Matrix3 Transpose() const;
			/**
			@return Inverse matrix
			*/
			Matrix3 Inverse() const;
			/**
			@return Cofactor matrix
			*/
			Matrix3 Cofactor() const;
			/**
			@return Adjunct matrix
			*/
			Matrix3 Adjunct() const;
			Matrix3 operator*(const Matrix3& m) const;
			Vector3 operator*(const Vector3& o) const;
			Matrix3 operator*(float scalar) const;


			/**
			@return Identity matrix
			*/
			static Matrix3 Identity();
			/**
			@param[in] x Scaling for x-axis
			@param[in] y Scaling for y-axis
			@param[in] z Scaling for z-axis
			@return Scale matrix
			*/
			static Matrix3 Scale(float x, float y, float z);
			/**
			@param[in] Vector2
			@return Translation matrix
			*/
			static Matrix3 Translation(const Vector2& t);
			/**
			@param[in] a Rotation in radians
			@return RotationX matrix
			*/
			static Matrix3 RotationX(float a);
			/**
			@param[in] a Rotation in radians
			@return RotationY matrix
			*/
			static Matrix3 RotationY(float a);
			/**
			@param[in] a Rotation in radians
			@return RotationZ matrix 
			*/
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