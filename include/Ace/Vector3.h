#pragma once

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
			
			
			/**
			@brief Constructor
			*/
			Vector3(float x = 0.f, float y = 0.f, float z = 0.f);
			
			/**
			@return Length
			*/
			float Length() const;
			
			/**
			@return Squared length
			*/
			float LengthSquared() const;
			
			/**
			@return Normalized vector
			*/
			Vector3 Normalize() const;
			
			Vector3 operator+(const Vector3& v) const;
			
			Vector3& operator+=(const Vector3& v);
			
			Vector3 operator-(const Vector3& v) const;
			
			Vector3& operator-=(const Vector3& v);
			
			Vector3 operator*(float scalar) const;
			
			Vector3& operator*=(float scalar);
			
			Vector3 operator/(float scalar) const;
			
			Vector3& operator/=(float scalar);
			
			inline operator Vector2() const;
			
			/**
			@param[in] Vector3
			@return Cross product
			*/
			Vector3 Cross(const Vector3& vec1) const;
			
			/**
			@param[in] Vector3
			@return Dot product
			*/
			float Dot(const Vector3& vec1) const;
			
			float& operator[](UInt8 index);
			
			float operator[](UInt8 index) const;
		};
	}
}