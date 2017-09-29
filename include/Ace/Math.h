#pragma once

namespace ace
{
    namespace math
    {
		static const float PI = 3.141592654f;

		static const float epsilon = 0.0001f;

		/**
		@brief Convert degrees to radians
		@param[in] deg Degrees
		@return radians
		*/
		inline float Rad(float deg)
		{
			return deg / 180.0f * PI;
		}

		/**
		@brief Convert radians to degrees
		@param[in] rad Radians
		@return degrees
		*/
		inline float Deg(float rad)
		{
			return rad / PI * 180.0f;
		}

		inline float Lerp(float start, float end, float delta)
		{
			return start + delta * (end - start);
		}



		float Abs(float);
		float Acos(float);
		float Asin(float);
		float Atan(float);
		float Ceil(float);
		float Cos(float);
		float Cosh(float);
		float Exp(float);
		float Floor(float);
		float Fmod(float, float);
		float Log(float);
		float Log10(float);
		float Max(float, float);
		float Min(float, float);
		float Pow(float base, float exp);
		float Rand(float min, float max);
		float Root(float base, float exp);
		float Round(float);
		bool SameSign(float, float);
		float Sin(float);
		float Sinh(float);
		float Sqrt(float);
		float Tan(float);
		float Tanh(float);
    }
}