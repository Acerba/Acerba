#pragma once

#include <Ace/Vector3.h>
#include <Ace/Matrix2.h>
#include <Ace/Matrix3.h>
#include <Ace/Matrix4.h>
#include <Ace/Platform.h>

#if ACE_DEBUG
	#include <Ace/Log.h>
#endif

namespace ace
{
    namespace math
    {
        class Quaternion
        {
        public:

            Vector3 vector;
            float scalar;

			static Quaternion Identity();

			Quaternion(const Quaternion& quaternation = Identity());
            Quaternion(const Vector3& vector, float scalar);

			/**
			@brief Calculate quaternion using euler angles
			@param[in] x X-axis rotation in degrees
			@param[in] y Y-axis rotation in degrees
			@param[in] z Z-axis rotation in degrees
			@return Rotated quaternion
			*/
			static Quaternion Euler(float xDegree, float yDegree, float zDegree);
			
			static Quaternion LookAt(const Vector3& forward, const Vector3& up = Vector3(0.0f,1.0f,0.0f));

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

			#if ACE_DEBUG
				void Log(const char* msg = nullptr) const
				{
					Logger::LogDebug(msg, "S: ", scalar, "V: ", vector.x, vector.y, vector.z);
				}
			#endif
        };
    }
}