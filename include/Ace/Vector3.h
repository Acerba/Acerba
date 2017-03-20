#pragma once

#include <ace/Math.h>
#include <Ace/IntTypes.h>
#include <Ace/Vector2.h>


namespace ace
{
	namespace math
	{

		class Vector3
		{
		public:

			union
			{
				struct
				{
					float x;
					float y;
					float z;
				};

				float array[3];
			};


			Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
			
			/**
			@return Lenght
			*/

			float Length()
			{
				return Sqrt(x*x + y*y + z*z);
			}

			/**
			@return Normalized vector
			*/

			Vector3 Normalize()
			{
				Vector3 v = *this;

                const float l = v.Length();

				for (UInt32 i = 0u; i < 3u; ++i)
				{
					v.array[i] /= l;
				}
				return v;
			}


			Vector3 operator+(const Vector3& vec1) const
			{
				return{ x + vec1.x, y + vec1.y, z + vec1.z};
			}

			Vector3& operator+=(const Vector3& vec1)
			{
				x += vec1.x;
				y += vec1.y;
				z += vec1.z;

				return *this;
			}

			Vector3 operator-(const Vector3& vec1) const
			{
				return{ x - vec1.x, y - vec1.y, z - vec1.z};
			}

			Vector3& operator-=(const Vector3& vec1)
			{
				x -= vec1.x;
				y -= vec1.y;
				z -= vec1.z;

				return *this;
			}

			Vector3 operator*(float scalar) const
			{
				return{ x * scalar, y *scalar, z *scalar};
			}

			Vector3& operator*=(float scalar)
			{
				x *= scalar;
				y *= scalar;
				z *= scalar;

				return *this;
			}

			Vector3 operator/(float scalar) const
			{
				return{ x / scalar, y / scalar, z / scalar};
			}

			Vector3& operator/=(float scalar)
			{
				x /= scalar;
				y /= scalar;
				z /= scalar;

				return *this;
			}

			inline operator Vector2()const
			{
				return Vector2(x, y);
			}

			/**
			@param[in] Vector3
			@return Cross product
			*/

			Vector3 Cross(const Vector3& vec1) const
			{
				return{y * vec1.z - z * vec1.y,
					   z * vec1.x - x * vec1.z, 
					   x * vec1.y - y * vec1.x};
			}

			/**
			@param[in] Vector3
			@return Dot product
			*/

			float Dot(const Vector3& vec1) const
			{
				return{ x * vec1.x + y * vec1.y + z * vec1.z};
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