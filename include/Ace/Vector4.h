#pragma once

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

			/**
			@brief Constructor
			*/
			Vector4(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f);

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
			Vector4 Normalize() const;
			
			Vector4 operator+(const Vector4& v) const;
			
			Vector4& operator+=(const Vector4& v);
			
			Vector4 operator-(const Vector4& v) const;
			
			Vector4& operator-=(const Vector4& v);
			
			Vector4 operator*(float scalar) const;
			
			Vector4& operator*=(float scalar);
			
			Vector4 operator/(float scalar) const;
			
			Vector4& operator/=(float scalar);
			
			/**
			@param[in] v Vector4
			@return Dot product
			*/
			float Dot(const Vector4& v) const;
			
			float& operator[](UInt8 index);
			
			float operator[](UInt8 index) const;
		};
	}
}