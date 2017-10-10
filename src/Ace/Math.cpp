#include <Ace/Math.h>
#include <Ace/IntTypes.h>

#include <algorithm> // std::generate_n
#include <array>
#include <functional> // std::ref
#include <math.h>
#include <random>

namespace ace
{
	namespace math
	{

		std::mt19937& GetWarmEngine()
		{
			static std::mt19937 engine;
			static bool warm = false;
			if (!warm)
			{
				std::random_device device;
				std::array<Int32, 624u> data;
				std::generate_n(data.data(), data.size(), std::ref(device));
				std::seed_seq seed(data.begin(), data.end());
				engine.seed(seed);
				warm = true;
			}
			return engine;
		}

		float Acos(float a)
		{
			return acos(a);
		}

		float Asin(float a)
		{
			return asin(a);
		}

		float Atan(float a)
		{
			return atan(a);
		}

		float Ceil(float a)
		{
			return ceil(a);
		}

		float Cos(float a)
		{
			return cos(a);
		}

		float Cosh(float a)
		{
			return cosh(a);
		}

		float Exp(float a)
		{
			return exp(a);
		}

		float Floor(float a)
		{
			return floor(a);
		}

		float Fmod(float a, float b)
		{
			return fmod(a,b);
		}

		float Log(float a)
		{
			return log(a);
		}

		float Log10(float a)
		{
			return log10(a);
		}

		float Pow(float a, float b)
		{
			return pow(a,b);
		}

		float Rand(float min, float max)
		{
			return std::uniform_real_distribution<float>(
				Min(min, max),
				Max(min, max)
			)(GetWarmEngine());
		}

		float Root(float a, float b)
		{
			return pow(a, 1 / b);
		}

		float Round(float a)
		{
			return round(a);
		}

		float Sin(float a)
		{
			return sin(a);
		}

		float Sinh(float a)
		{
			return sinh(a);
		}

		float Sqrt(float a)
		{
			return sqrt(a);
		}

		float Tan(float a)
		{
			return tan(a);
		}

		float Tanh(float a)
		{
			return tanh(a);
		}

	}
}
