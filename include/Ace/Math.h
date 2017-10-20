#pragma once

namespace ace
{
    namespace math
    {
        static const float PI = 3.141592654f;

        static const float epsilon = 0.001f;


        inline float Abs(float a)
        {
            return a >= 0.f ? a : -a;
        }
        float Acos(float);
        float Asin(float);
        float Atan(float);
        float Ceil(float);
        inline float Clamp(float value, float min, float max)
        {
            return min <= value ? (value <= max ? value : max) : min;
        }
        float Cos(float);
        float Cosh(float);
        /**
            @brief Convert radians to degrees
            @param[in] rad Radians
            @return degrees
        */
        inline float Deg(float rad)
        {
            return rad / PI * 180.0f;
        }
        float Exp(float);
        float Floor(float);
        float Fmod(float, float);
        inline bool IsBetween(float value, float min, float max)
        {
            return min <= value && value <= max;
        }
        inline bool IsSameSign(float a, float b)
        {
            return (a <= 0.f && b <= 0.f) || (a >= 0.f && b >= 0.f);
        }
        inline float Lerp(float start, float end, float delta)
        {
            return start + delta * (end - start);
        }
        float Log(float);
        float Log10(float);
        inline float Max(float a, float b)
		{
			return a < b ? b : a;
		}
		inline float Min(float a, float b)
		{
			return a < b ? a : b;
        }
        inline bool IsNearEpsilon(float value)
        {
            return IsBetween(value, -epsilon, epsilon);
        }
        float Pow(float base, float exp);
        /**
            @brief Convert degrees to radians
            @param[in] deg Degrees
            @return radians
        */
        inline float Rad(float deg)
        {
            return deg / 180.0f * PI;
        }
        float Rand(float min, float max);
        float Root(float base, float exp);
        float Round(float);
        float Sin(float);
        float Sinh(float);
        float Sqrt(float);
        float Tan(float);
        float Tanh(float);
    }
}