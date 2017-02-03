#pragma once

#include <ace/Math.h>
#include <Ace/IntTypes.h>

namespace ace
{
	namespace math
	{

		class Vector4
		{
		public:

			union
			{
				struct
				{
					float x;
					float y;
					float z;
					float w;
				};

				float array[4];
			};

			

			Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}

			float Lenght()
			{
				return Sqrt(x*x + y*y + z*z + w*w);
			}

			Vector4 operator+(const Vector4& vec1) const
			{
				return{ x + vec1.x, y + vec1.y, z + vec1.z, w + vec1.w };
			}

			Vector4& operator+=(const Vector4& vec1)
			{
				x += vec1.x;
				y += vec1.y;
				z += vec1.z;
				w += vec1.w;

				return *this;
			}

			Vector4 operator-(const Vector4& vec1) const
			{
				return{ x - vec1.x, y - vec1.y, z - vec1.z, w - vec1.w };
			}

			Vector4& operator-=(const Vector4& vec1)
			{
				x -= vec1.x;
				y -= vec1.y;
				z -= vec1.z;
				w -= vec1.w;

				return *this;
			}

			Vector4 operator*(float scalar) const
			{
				return{ x * scalar, y *scalar, z *scalar, w *scalar};
			}

			Vector4& operator*=(float scalar)
			{
				x *= scalar;
				y *= scalar;
				z *= scalar;
				w *= scalar;

				return *this;
			}

			Vector4 operator/(float scalar) const
			{
				return{ x / scalar, y / scalar, z / scalar, w / scalar};
			}

			Vector4& operator/=(float scalar)
			{
				x /= scalar;
				y /= scalar;
				z /= scalar;
				z /= scalar;

				return *this;
			}


			float Dot(const Vector4& vec1) const
			{
				return{ x * vec1.x + y * vec1.y + z * vec1.z + w * vec1.w};
			}


			float& operator[](UInt8 index)
			{
				return array[index];
			}

			float operator[](UInt8 index)const
			{
				return array[index];
			}

		};
	}
}