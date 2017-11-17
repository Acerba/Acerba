#pragma once

#include <Ace/IntTypes.h>

namespace ace
{
	struct Color32;
	struct Color;

	struct BasicColors final
	{
		static const UInt32 Red = 0xFF000000U;
		static const UInt32 Green = 0x00FF0000U;
		static const UInt32 Blue = 0x0000FF00U;

		BasicColors() = delete;
		~BasicColors() = delete;
		BasicColors(BasicColors&&) = delete;
		BasicColors(const BasicColors&) = delete;
		BasicColors& operator=(BasicColors&&) = delete;
		BasicColors& operator=(const BasicColors&) = delete;
	};

	struct Color final
	{
		UInt8 r, g, b, a;

		explicit Color(UInt8 r = 0u, UInt8 g = 0u, UInt8 b = 0u, UInt8 a = 255u) : r(r), g(g), b(b), a(a)
		{

		}

		Color(UInt32 hex)
		{
			r = (hex & 0xFF000000) >> 24;
			g = (hex & 0x00FF0000) >> 16;
			b = (hex & 0x0000FF00) >> 8;
			a = (hex & 0x000000FF) >> 0;
		}
	
		Color operator+(const Color& c) const
		{
			return Color(r + c.r, g + c.g, b + c.b, a + c.a);
		}

		Color operator-(const Color& c) const
		{
			return Color(r - c.r, g - c.g, b - c.b, a - c.a);
		}
		

		Color operator*(float scalar) const
		{
			return Color(
				static_cast<UInt8>(static_cast<float>(r) * scalar),
				static_cast<UInt8>(static_cast<float>(g) * scalar),
				static_cast<UInt8>(static_cast<float>(b) * scalar),
				static_cast<UInt8>(static_cast<float>(a) * scalar)
			);
		}

		Color operator/(float scalar) const
		{
			return Color(
				static_cast<UInt8>(static_cast<float>(r) / scalar),
				static_cast<UInt8>(static_cast<float>(g) / scalar),
				static_cast<UInt8>(static_cast<float>(b) / scalar),
				static_cast<UInt8>(static_cast<float>(a) / scalar)
			);
		}

		operator Color32() const;

	}; // Color

	struct Color32 final
	{
		float r, g, b, a;
		explicit Color32(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) : r(r), g(g), b(b), a(a)
		{
			
		}

		Color32(UInt32 hex)
		{
			Color c(hex);
			*this = c; //<--Don't worry about this :D
		}
		Color32 operator+(const Color32& c32) const
		{
			return Color32(r + c32.r, g + c32.g, b + c32.b, a + c32.a);
		}

		Color32 operator-(const Color32& c32) const
		{
			return Color32(r - c32.r, g - c32.g, b - c32.b, a - c32.a);
		}

		Color32 operator*(float scalar) const
		{
			return Color32(r * scalar, g * scalar, b * scalar, a * scalar);
		}

		Color32 operator/(float scalar) const
		{
			return Color32(r / scalar, g / scalar, b / scalar, a / scalar);
		}

		operator Color() const;
	}; // Color32

	/**
		@brief Conversion from Color to Color32
		@return Color32
	*/
	inline Color::operator ace::Color32() const
	{
		return Color32(
			static_cast<UInt8>(static_cast<float>(r) / 255.0f),
			static_cast<UInt8>(static_cast<float>(g) / 255.0f),
			static_cast<UInt8>(static_cast<float>(b) / 255.0f),
			static_cast<UInt8>(static_cast<float>(a) / 255.0f)
		);
	}
	
	/**
		@brief Conversion from Color32 to Color
		@return Color
	*/
	inline Color32::operator ace::Color() const
	{
		return Color(
			static_cast<UInt8>(r * 255.f),
			static_cast<UInt8>(g * 255.f),
			static_cast<UInt8>(b * 255.f),
			static_cast<UInt8>(a * 255.f)
		);
	}

}