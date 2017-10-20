#pragma once

#include <Matrix.hpp>

namespace ace
{
	namespace math
	{
		using Matrix4 = mv::Mat<4u, 4u, float>;

		static const Matrix4 s_identity4 = mv::MakeIdentity<4u, float>();
	}
}