#pragma once

#include <Matrix.hpp>

namespace ace
{
	namespace math
	{
		using namespace mv;
		using Matrix4 = Mat<4u, 4u, float>;

		static const Matrix4 s_identity4 = MakeIdentity<4u, float>();

		static Matrix4 RotateZ4(float deg)
		{
			return MakeRotation<4u>(ToRad(deg), AXIS::Z);
		}
	}
}