#pragma once

#include <Ace/Vector3.h>

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

            Quaternion operator+(const Quaternion& q1) const
            {
                return{ vector + q1.vector, scalar + q1.scalar };
            }

            Quaternion& operator+=(const Quaternion& q1)
            {
                vector += q1.vector;
                scalar += q1.scalar;

                return *this;
            }

            Quaternion operator-(const Quaternion& q1) const
            {
                return{ vector - q1.vector, scalar - q1.scalar };
            }

            Quaternion& operator-=(const Quaternion& q1)
            {
                vector -= q1.vector;
                scalar -= q1.scalar;

                return *this;
            }

        };
    }
}