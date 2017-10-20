#pragma once

#include <Matrix.hpp>

namespace ace
{
	namespace math
	{
		using Matrix4 = mv::Mat<4u, 4u, float>;

		static const Matrix4 s_identity4 = mv::MakeIdentity<4u, float>();

		static Matrix4 RotateZ4(float deg)
		{
			return mv::MakeRotation<4u>(mv::ToRad(deg), mv::AXIS::Z);
		}
	}
}