#pragma once

#include <Ace/Vector3.h>
#include <Ace/Matrix2.h>
#include <Ace/Matrix3.h>
#include <Ace/Matrix4.h>

namespace ace
{
    namespace math
    {
        class Quaternion
        {
        public:

            Vector3 vector;
            float scalar;

            Quaternion(const Vector3& vector = Vector3(), float scalar = 0.f);

			/**
			@brief Calculate quaternion using euler angles
			@param[in] x X-axis rotation in degrees
			@param[in] y Y-axis rotation in degrees
			@param[in] z Z-axis rotation in degrees
			@return Rotated quaternion
			*/
			static Quaternion Euler(float xDegree, float yDegree, float zDegree);

			Quaternion operator*(const Quaternion& q1) const;

			Quaternion& operator*=(const Quaternion& q1);

			Quaternion operator+(const Quaternion& q1) const;

			Quaternion& operator+=(const Quaternion& q1);

			Quaternion operator-(const Quaternion& q1) const;

			Quaternion& operator-=(const Quaternion& q1);

			/**
			@brief Convert quaternion to matrix2
			@return matrix2
			*/
			Matrix2 ToMatrix2() const;

			/**
			@brief Convert quaternion to matrix3
			@return matrix3
			*/
			Matrix3 ToMatrix3() const;

			/**
			@brief Convert quaternion to matrix4
			@return matrix4
			*/
			Matrix4 ToMatrix4() const;
        };
    }
}