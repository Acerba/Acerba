#pragma once

#include <Ace/Vector2.h>
#include <Ace/IntTypes.h>

namespace ace
{
	namespace math
	{

		class Matrix2 final
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
			Matrix2(float identity);
			Matrix2(const Vector2& v1, const Vector2& v2);

			/**
			@return Determinant
			*/
			float Determinant() const;
			/**
			@return Transpose matrix
			*/
			Matrix2 Transpose() const;
			/**
			@return Inverse matrix
			*/
			Matrix2 Inverse() const;
			/**
			@return Cofactor matrix
			*/
			Matrix2 Cofactor() const;
			/**
			@return Adjunct matrix
			*/
			Matrix2 Adjunct() const;
			Matrix2 operator*(const Matrix2& m) const;
			Vector2 operator*(const Vector2& o) const;
			Matrix2 operator*(float scalar) const;


			/**
			@return Identity matrix
			*/
			static Matrix2 Identity();
			/**
			@param[in] x Scaling for x-axis
			@param[in] y Scaling for y-axis
			@return Scale matrix
			*/
			static Matrix2 Scale(float x, float y);
			/**
			@param[in] deg Rotation in degrees
			@return Rotation matrix
			*/
			static Matrix2 Rotation(float deg);
			


			/**
				@brief Retrieve indexth row of the matrix by reference.
				@param[in] index Index of the row.
				@warning Undefined behaviour if index >= dimension of the matrix.
				@return Row by reference.
			*/
			Vector2& operator[](UInt8 index)
			{
				return rows[index];
			}

			/**
				@brief Retrieve indexth row of the matrix.
				@param[in] index Index of the row.
				@warning Undefined behaviour if index >= dimension of the matrix.
				@return Row by value.
			*/
			Vector2 operator[](UInt8 index) const
			{
				return rows[index];
			}

			/**
				@brief Retrieve reference to the value in the coordinates of the row-major matrix.
				@param[in] r Row index.
				@param[in] c Column index.
				@warning Undefined behaviour if either argument >= dimension of the matrix.
				@return Value by reference.
			*/
			float& operator()(UInt8 r, UInt8 c)
			{
				return data[r][c];
			}

			/**
				@brief Retrieve value in the coordinates of the row-major matrix.
				@param[in] r Row index.
				@param[in] c Column index.
				@warning Undefined behaviour if either argument >= dimension of the matrix.
				@return Value.
			*/
			float operator()(UInt8 r, UInt8 c)const
			{
				return data[r][c];
			}

		};

	}
}