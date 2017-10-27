#pragma once

#include <Matrix.hpp>

namespace ace
{
	namespace math
	{
		using namespace mv;
		using Matrix2 = Mat<2u, 2u, float>;

		// Can not use values that come from template function calls as function default arguments in VS.
		static const Matrix2 s_identity2 = MakeIdentity<2u, float>();

		static Matrix2 RotateZ2(float deg)
		{
			return MakeRotation<2u>(ToRad(deg), AXIS::Z);
		}
	}
}