#pragma once

namespace ace
{
    namespace math
    {
		const float PI = 3.14f;

		inline float Rad(float deg)
		{
			return deg / 180.0f * PI;
		}

		inline float Deg(float rad)
		{
			return rad / PI * 180.0f;
		}

		inline float Lerp(float start, float end, float delta)
		{
			return start + delta * (end - start);
		}

		float Abs(float);
	    float Pow(float base, float exp);
		float Sqrt(float);
		float Sin(float);
		float Cos(float);
		float Tan(float);
		float Asin(float);
		float Acos(float);
		float Atan(float);
		float Ceil(float);
		float Floor(float);
		float Exp(float);
		float Round(float);
		float Log(float);
		float Log10(float);
		float Fmod(float, float);
		float Root(float base, float exp);
		float Sinh(float);
		float Cosh(float);
		float Tanh(float);
    }
}