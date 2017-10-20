#pragma once

#include <Matrix.hpp>

namespace ace
{
	namespace math
	{
		using Matrix3 = mv::Mat<3u, 3u, float>;

		static const Matrix3 s_identity3 = mv::MakeIdentity<3u, float>();
	}
}