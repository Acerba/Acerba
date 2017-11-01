#pragma once

#include <Matrix.hpp>

namespace ace
{
	namespace math
	{
		using namespace mv;
		using Matrix3 = Mat<3u, 3u, float>;

		static const Matrix3 s_identity3 = MakeIdentity<3u, float>();
	}
}