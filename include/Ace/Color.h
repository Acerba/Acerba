#pragma once

namespace ace
{
	/*
		Color (RGBA)
		-> UInt8 (unsigned char) 0...255
		-> Operators +- Color 
			-> * / Scalar

		Color32 (RGBA)
		-> float 0...1 (HDR)
		-> Operators +- Color
			-> * / Scalar

		Color c = ...;
		Color c32 = ...;

		Hex Constructor
		r = (hex & 0xFF000000) >> 24; 

		c = c32;
		c32 = c;
	*/

	struct Color
	{
		UInt8 r, g, b, a;
	};

	struct Color32
	{
		float r, g, b, a;
	};

}