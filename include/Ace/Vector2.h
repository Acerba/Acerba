#pragma once

namespace ace
{
	namespace math
	{
		class Vector2 final
		{
		public:
			
			float x;
			float y;
			
			/**
			@brief Constructor
			*/
			Vector2(float x = 0.f, float y = 0.f);
			
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
			Vector2 Normalize() const;
			
			Vector2 operator+(const Vector2& v) const;
			
			Vector2& operator+=(const Vector2& v);
			
			Vector2 operator-(const Vector2& v) const;
			
			Vector2& operator-=(const Vector2& v);
			
			Vector2 operator*(float scalar) const;
			
			Vector2& operator*=(float scalar);
			
			Vector2 operator/(float scalar) const;
			
			Vector2& operator/=(float scalar);
			
			/**
			@param[in] v Vector2
			@return Cross product
			*/
			float Cross(const Vector2& v) const;
			
			/**
			@param[in] a Vector2
			@param[in] b Vector2
			@return Cross product
			*/
			static float Cross(const Vector2& a, const Vector2& b);

			/**
			@param[in] v Vector2
			@return Dot product
			*/
			float Dot(const Vector2& v) const;

			/**
			@param[in] a Vector2
			@param[in] b Vector2
			@return Dot product
			*/
			static float Dot(const Vector2& a, const Vector2& b);

			/**
			@return Returns vector with each element sign changed.
			*/
			Vector2 Invert() const;

		};
	}
}