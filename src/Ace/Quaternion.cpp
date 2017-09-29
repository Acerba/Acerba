#include <Ace/Quaternion.h>
#include <Ace/Math.h>

namespace ace
{
	namespace math
	{

		Quaternion::Quaternion(const Vector3& vector, float scalar):
			vector(vector), scalar(scalar)
		{

		}

		Quaternion::Quaternion(const Quaternion& quaternation) :
			vector(quaternation.vector), scalar(quaternation.scalar)
		{

		}

		Quaternion Quaternion::Identity()
		{
			return Quaternion(Vector3(0, 0, 0), 1.0f);
		}

		Quaternion Quaternion::Euler(float xDegree, float yDegree, float zDegree)
		{
			const float x = Rad(xDegree * 0.5f);
			const float y = Rad(yDegree * 0.5f);
			const float z = Rad(zDegree * 0.5f);

			return
				Quaternion(Vector3(Sin(x), 0, 0), Cos(x))*
				Quaternion(Vector3(0, Sin(y), 0), Cos(y))*
				Quaternion(Vector3(0, 0, Sin(z)), Cos(z));
		}

		Quaternion Quaternion::LookAt(const Vector3& forward, const Vector3& up)
		{
			Vector3 vector = forward.Normalize();
			Vector3 vector2 = Vector3::Cross(up, vector).Normalize();
			Vector3 vector3 = Vector3::Cross(vector, vector2);
			
			float m00 = vector2.x;
			float m01 = vector2.y;
			float m02 = vector2.z;
			float m10 = vector3.x;
			float m11 = vector3.y;
			float m12 = vector3.z;
			float m20 = vector.x;
			float m21 = vector.y;
			float m22 = vector.z;


			float num8 = (m00 + m11) + m22;
			Quaternion quaternion;

			if (num8 > 0.0f)
			{
				float num = math::Sqrt(num8 + 1.0f);

				quaternion.scalar = num * 0.5f;
				num = 0.5f / num;
				quaternion.vector.x = (m12 - m21) * num;
				quaternion.vector.y = (m20 - m02) * num;
				quaternion.vector.z = (m01 - m10) * num;
				return quaternion;
			}

			if ((m00 >= m11) && (m00 >= m22))
			{
				float num7 = math::Sqrt(((1.0f + m00) - m11) - m22);
				float num4 = 0.5f / num7;
				quaternion.vector.x = 0.5f * num7;
				quaternion.vector.y = (m01 + m10) * num4;
				quaternion.vector.z = (m02 + m20) * num4;
				quaternion.scalar = (m12 - m21) * num4;
				return quaternion;
			}

			if (m11 > m22)
			{
				float num6 = math::Sqrt(((1.0f + m11) - m00) - m22);
				float num3 = 0.5f / num6;
				quaternion.vector.x = (m10 + m01) * num3;
				quaternion.vector.y = 0.5f * num6;
				quaternion.vector.z = (m21 + m12) * num3;
				quaternion.scalar = (m20 - m02) * num3;
				return quaternion;
			}

			float num5 = math::Sqrt(((1.0f + m22) - m00) - m11);
			float num2 = 0.5f / num5;
			quaternion.vector.x = (m20 + m02) * num2;
			quaternion.vector.y = (m21 + m12) * num2;
			quaternion.vector.z = 0.5f * num5;
			quaternion.scalar = (m01 - m10) * num2;
			return quaternion;
		}

		Quaternion Quaternion::operator*(const Quaternion& q1) const
		{
			return{ vector.Cross(q1.vector) + q1.vector*scalar + vector*q1.scalar, scalar*q1.scalar - vector.Dot(q1.vector)};
		}

		Quaternion& Quaternion::operator*=(const Quaternion& q1)
		{
			vector = vector.Cross(q1.vector) + q1.vector*scalar + vector*q1.scalar;
			scalar = scalar*q1.scalar - vector.Dot(q1.vector);

			return *this;
		}

		Quaternion Quaternion::operator+(const Quaternion& q1) const
		{
			return{ vector + q1.vector, scalar + q1.scalar };
		}

		Quaternion& Quaternion::operator+=(const Quaternion& q1)
		{
			vector += q1.vector;
			scalar += q1.scalar;

			return *this;
		}

		Quaternion Quaternion::operator-(const Quaternion& q1) const
		{
			return{ vector - q1.vector, scalar - q1.scalar };
		}

		Quaternion& Quaternion::operator-=(const Quaternion& q1)
		{
			vector -= q1.vector;
			scalar -= q1.scalar;

			return *this;
		}


		Matrix2 Quaternion::ToMatrix2() const
		{
			return Matrix2(
				Vector2(2 * (Pow(scalar, 2) + Pow(vector.x, 2)) - 1, 2 * (vector.x*vector.y - scalar*vector.z)),
				Vector2(2 * (vector.x*vector.y + scalar*vector.z), 2 * (Pow(scalar, 2) + Pow(vector.y, 2)) - 1)
			);
		}

		Matrix3 Quaternion::ToMatrix3() const
		{
			return Matrix3(
				Vector3(2 * (Pow(scalar, 2) + Pow(vector.x, 2)) - 1, 2 * (vector.x*vector.y - scalar*vector.z), 2 * (vector.x*vector.z + scalar*vector.y)),
				Vector3(2 * (vector.x*vector.y + scalar*vector.z), 2 * (Pow(scalar, 2) + Pow(vector.y, 2)) - 1, 2 * (vector.y*vector.z - scalar*vector.x)),
				Vector3(2 * (vector.x*vector.z - scalar*vector.y), 2 * (vector.y*vector.z + scalar*vector.x), 2 * (Pow(scalar, 2) + Pow(vector.z, 2)) - 1)
			);
		}

		Matrix4 Quaternion::ToMatrix4() const
		{
			return Matrix4(
				Vector4(2 * (Pow(scalar, 2) + Pow(vector.x, 2)) - 1, 2 * (vector.x*vector.y - scalar*vector.z), 2 * (vector.x*vector.z + scalar*vector.y), 0),
				Vector4(2 * (vector.x*vector.y + scalar*vector.z), 2 * (Pow(scalar, 2) + Pow(vector.y, 2)) - 1, 2 * (vector.y*vector.z - scalar*vector.x), 0),
				Vector4(2 * (vector.x*vector.z - scalar*vector.y), 2 * (vector.y*vector.z + scalar*vector.x), 2 * (Pow(scalar, 2) + Pow(vector.z, 2)) - 1, 0),
				Vector4(0, 0, 0, 1)
			);
		}
	}

}