#include <Ace\Math.h>
#include <math.h>


namespace ace
{
	namespace math
	{
		float Abs(float a)
		{
			return abs(a);
		}

		float Pow(float a, float b)
		{
			return pow(a,b);
		}

		float Sqrt(float a)
		{
			return sqrt(a);
		}

		float Sin(float a)
		{
			return sin(a);
		}

		float Cos(float a)
		{
			return cos(a);
		}

		float Tan(float a)
		{
			return tan(a);
		}

		float Asin(float a)
		{
			return asin(a);
		}

		float Acos(float a)
		{
			return acos(a);
		}

		float Atan(float a)
		{
			return atan(a);
		}
		
		float Ceil(float a)
		{
			return ceil(a);
		}

		float Floor(float a)
		{
			return floor(a);
		}

		float Exp(float a)
		{
			return exp(a);
		}

		float Round(float a)
		{
			return round(a);
		}

		float Log(float a)
		{
			return log(a);
		}

		float Log10(float a)
		{
			return log10(a);
		}

		float Fmod(float a, float b)
		{
			return fmod(a,b);
		}

		float Root(float a, float b)
		{
			return pow(a, 1 / b);
		}

		float Sinh(float a)
		{
			return sinh(a);
		}

		float Cosh(float a)
		{
			return cosh(a);
		}

		float Tanh(float a)
		{
			return tanh(a);
		}
	}
}