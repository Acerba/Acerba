#pragma once

#include <ace/Math.h>



namespace ace
{
	namespace math
	{

		class Vector2
		{
		public:

			float x;
			float y;

			Vector2(float x = 0.f, float y = 0.f) : x(x), y(y) {}

			/**
			@return Lenght
			*/

			float Lenght()
			{
				return Sqrt(x*x + y*y);
			}

			Vector2 operator+(const Vector2& vec1) const
			{
				return{ x + vec1.x, y + vec1.y };
			}
		    
			Vector2& operator+=(const Vector2& vec1)
			{
				x += vec1.x;
				y += vec1.y;

				return *this;
			}

			Vector2 operator-(const Vector2& vec1) const
			{
				return{ x - vec1.x, y - vec1.y };
			}

			Vector2& operator-=(const Vector2& vec1)
			{
				x -= vec1.x;
				y -= vec1.y;

				return *this;
			}

			Vector2 operator*(float scalar) const
			{
				return{ x * scalar, y *scalar};
			}

			Vector2& operator*=(float scalar)
			{
				x *= scalar;
				y *= scalar;

				return *this;
			}
			
			Vector2 operator/(float scalar) const
			{
				return{ x / scalar, y / scalar};
			}

			Vector2& operator/=(float scalar)
			{
				x /= scalar;
				y /= scalar;

				return *this;
			}

			/**
			@param[in] Vector2
			@return Cross product
			*/

			float Cross(const Vector2& vec1) const
			{
				return{ x * vec1.y - y * vec1.x};
			}

			/**
			@param[in] Vector2
			@return Dot product
			*/

			float Dot(const Vector2& vec1) const
			{
				return{ x * vec1.x + y * vec1.y };
			}
		};
	}
}