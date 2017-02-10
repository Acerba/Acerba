#pragma once

#include <Ace/Math.h>
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

            Quaternion(const Vector3& vector = Vector3(), float scalar = 0.f):
                vector(vector),
                scalar(scalar)
            {

            }

			static Quaternion Euler(float xDegree, float yDegree, float zDegree)
			{
				float x = Rad(xDegree / 2.0f);
				float y = Rad(yDegree / 2.0f);
				float z = Rad(zDegree / 2.0f);

				return
					Quaternion(Vector3(Sin(x), 0, 0), Cos(x))*
					Quaternion(Vector3(0, Sin(y), 0), Cos(y))*
					Quaternion(Vector3(0, 0, Sin(z)), Cos(z));
			}
		
			
			Quaternion operator*(const Quaternion& q1) const;
			
			Quaternion& operator*=(const Quaternion& q1);
			
			Quaternion operator+(const Quaternion& q1) const;
          
			Quaternion& operator+=(const Quaternion& q1);
           
			Quaternion operator-(const Quaternion& q1) const;
           
			Quaternion& operator-=(const Quaternion& q1);
           
			Matrix2 ToMatrix2() const;

			Matrix3 ToMatrix3() const;

			Matrix4 ToMatrix4() const;
        };
    }
}