#pragma once


namespace ace
{
	namespace math
	{

		class Vector2
		{
		public:

			float x;
			float y;

			Vector2(float x = 0, float y = 0) : x(x), y(y) {}

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

			float Cross(const Vector2& vec1) const
			{
				return{ x * vec1.y - y * vec1.x};
			}

			float Dot(const Vector2& vec1) const
			{
				return{ x * vec1.x + y * vec1.y };
			}
		};
	}
}