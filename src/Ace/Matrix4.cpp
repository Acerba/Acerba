#include <Ace/Matrix4.h>

namespace ace
{
    namespace math
    {

        Matrix4::Matrix4() : rows() {}

        Matrix4::Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
        {
            rows[0] = v1;
            rows[1] = v2;
            rows[2] = v3;
            rows[3] = v4;
        }

        Matrix4::Matrix4(float identity) : rows()
        {
            data[0][0] = data[1][1] = data[2][2] = data[3][3] = identity;
        }

        float Matrix4::Determinant() const
        {
            Matrix4 m = *this;

            return
                m(0, 3) * m(1, 2) * m(2, 1) * m(3, 0) - m(0, 2) * m(1, 3) * m(2, 1) * m(3, 0) -
                m(0, 3) * m(1, 1) * m(2, 2) * m(3, 0) + m(0, 1) * m(1, 3) * m(2, 2) * m(3, 0) +
                m(0, 2) * m(1, 1) * m(2, 3) * m(3, 0) - m(0, 1) * m(1, 2) * m(2, 3) * m(3, 0) -
                m(0, 3) * m(1, 2) * m(2, 0) * m(3, 1) + m(0, 2) * m(1, 3) * m(2, 0) * m(3, 1) +
                m(0, 3) * m(1, 0) * m(2, 2) * m(3, 1) - m(0, 0) * m(1, 3) * m(2, 2) * m(3, 1) -
                m(0, 2) * m(1, 0) * m(2, 3) * m(3, 1) + m(0, 0) * m(1, 2) * m(2, 3) * m(3, 1) +
                m(0, 3) * m(1, 1) * m(2, 0) * m(3, 2) - m(0, 1) * m(1, 3) * m(2, 0) * m(3, 2) -
                m(0, 3) * m(1, 0) * m(2, 1) * m(3, 2) + m(0, 0) * m(1, 3) * m(2, 1) * m(3, 2) +
                m(0, 1) * m(1, 0) * m(2, 3) * m(3, 2) - m(0, 0) * m(1, 1) * m(2, 3) * m(3, 2) -
                m(0, 2) * m(1, 1) * m(2, 0) * m(3, 3) + m(0, 1) * m(1, 2) * m(2, 0) * m(3, 3) +
                m(0, 2) * m(1, 0) * m(2, 1) * m(3, 3) - m(0, 0) * m(1, 2) * m(2, 1) * m(3, 3) -
                m(0, 1) * m(1, 0) * m(2, 2) * m(3, 3) + m(0, 0) * m(1, 1) * m(2, 2) * m(3, 3);
        }


        Matrix4 Matrix4::Transpose() const
        {
            Matrix4 t;

            t(0, 0) = (*this)(0, 0);
            t(1, 1) = (*this)(1, 1);
            t(2, 2) = (*this)(2, 2);
            t(3, 3) = (*this)(3, 3);
            t(1, 2) = (*this)(2, 1);
            t(2, 1) = (*this)(1, 2);
            t(0, 1) = (*this)(1, 0);
            t(0, 2) = (*this)(2, 0);
            t(1, 0) = (*this)(0, 1);
            t(2, 0) = (*this)(0, 2);
            t(3, 0) = (*this)(0, 3);
            t(0, 3) = (*this)(3, 0);
            t(3, 1) = (*this)(1, 3);
            t(1, 3) = (*this)(3, 1);
            t(3, 2) = (*this)(2, 3);
            t(2, 3) = (*this)(3, 2);

            return t;
        }

        Matrix4 Matrix4::Inverse() const
        {
            return Adjunct() * (1 / Determinant());
        }

        Matrix4 Matrix4::Cofactor() const
        {
            Matrix4 m;

            m(0, 0) = data[1][1] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) - data[1][2] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) + data[1][3] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]);
            m(0, 1) = -1 * (data[1][0] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) - data[1][2] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) + data[1][3] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]));
            m(0, 2) = data[1][0] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) - data[1][1] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) + data[1][3] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]);
            m(0, 3) = -1 * (data[1][0] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]) - data[1][1] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]) + data[1][2] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]));

            m(1, 0) = -1 * (data[0][1] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) - data[0][2] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) + data[0][3] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]));
            m(1, 1) = data[0][0] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) - data[0][2] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) + data[0][3] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]);
            m(1, 2) = -1 * (data[0][0] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) - data[0][1] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) + data[0][3] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]));
            m(1, 3) = data[0][0] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]) - data[0][1] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]) + data[0][2] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]);

            m(2, 0) = data[0][1] * (data[1][2] * data[3][3] - data[1][3] * data[3][2]) - data[0][2] * (data[1][1] * data[3][3] - data[1][3] * data[3][1]) + data[0][3] * (data[1][1] * data[3][2] - data[1][2] * data[3][1]);
            m(2, 1) = -1 * (data[0][0] * (data[1][2] * data[3][3] - data[1][3] * data[3][2]) - data[0][2] * (data[1][0] * data[3][3] - data[1][3] * data[3][0]) + data[0][3] * (data[1][0] * data[3][2] - data[1][2] * data[3][0]));
            m(2, 2) = data[0][0] * (data[1][1] * data[3][3] - data[1][3] * data[3][1]) - data[0][1] * (data[1][0] * data[3][3] - data[1][3] * data[3][0]) + data[0][3] * (data[1][0] * data[3][1] - data[1][1] * data[3][0]);
            m(2, 3) = -1 * (data[0][0] * (data[1][1] * data[3][2] - data[1][2] * data[3][1]) - data[0][1] * (data[1][0] * data[3][2] - data[1][2] * data[3][0]) + data[0][2] * (data[1][0] * data[3][1] - data[1][1] * data[3][0]));

            m(3, 0) = -1 * (data[0][1] * (data[1][2] * data[2][3] - data[1][3] * data[2][2]) - data[0][2] * (data[1][1] * data[2][3] - data[1][3] * data[2][1]) + data[0][3] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]));
            m(3, 1) = data[0][0] * (data[1][2] * data[2][3] - data[1][3] * data[2][2]) - data[0][2] * (data[1][0] * data[2][3] - data[1][3] * data[2][0]) + data[0][3] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]);
            m(3, 2) = -1 * (data[0][0] * (data[1][1] * data[2][3] - data[1][3] * data[2][1]) - data[0][1] * (data[1][0] * data[2][3] - data[1][3] * data[2][0]) + data[0][3] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]));
            m(3, 3) = data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) - data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) + data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);

            return m;
        }

        Matrix4 Matrix4::Adjunct() const
        {
            Matrix4 adj = Cofactor().Transpose();
            return adj;
        }

        Matrix4 Matrix4::operator*(const Matrix4& m) const
        {
            return Matrix4(Vector4(rows[0].x*m(0, 0) + rows[0].y*m(1, 0) + rows[0].z*m(2, 0) + rows[0].w*m(3, 0), rows[0].x*m(0, 1) + rows[0].y*m(1, 1) + rows[0].z*m(2, 1) + rows[0].w*m(3, 1), rows[0].x*m(0, 2) + rows[0].y*m(1, 2) + rows[0].z*m(2, 2) + rows[0].w*m(3, 2), rows[0].x*m(0, 3) + rows[0].y*m(1, 3) + rows[0].z*m(2, 3) + rows[0].w*m(3, 3)),
                Vector4(rows[1].x*m(0, 0) + rows[1].y*m(1, 0) + rows[1].z*m(2, 0) + rows[1].w*m(3, 0), rows[1].x*m(0, 1) + rows[1].y*m(1, 1) + rows[1].z*m(2, 1) + rows[1].w*m(3, 1), rows[1].x*m(0, 2) + rows[1].y*m(1, 2) + rows[1].z*m(2, 2) + rows[1].w*m(3, 2), rows[1].x*m(0, 3) + rows[1].y*m(1, 3) + rows[1].z*m(2, 3) + rows[1].w*m(3, 3)),
                Vector4(rows[2].x*m(0, 0) + rows[2].y*m(1, 0) + rows[2].z*m(2, 0) + rows[2].w*m(3, 0), rows[2].x*m(0, 1) + rows[2].y*m(1, 1) + rows[2].z*m(2, 1) + rows[2].w*m(3, 1), rows[2].x*m(0, 2) + rows[2].y*m(1, 2) + rows[2].z*m(2, 2) + rows[2].w*m(3, 2), rows[2].x*m(0, 3) + rows[2].y*m(1, 3) + rows[2].z*m(2, 3) + rows[2].w*m(3, 3)),
                Vector4(rows[3].x*m(0, 0) + rows[3].y*m(1, 0) + rows[3].z*m(2, 0) + rows[3].w*m(3, 0), rows[3].x*m(0, 1) + rows[3].y*m(1, 1) + rows[3].z*m(2, 1) + rows[3].w*m(3, 1), rows[3].x*m(0, 2) + rows[3].y*m(1, 2) + rows[3].z*m(2, 2) + rows[3].w*m(3, 2), rows[3].x*m(0, 3) + rows[3].y*m(1, 3) + rows[3].z*m(2, 3) + rows[3].w*m(3, 3)));
        }

        Vector4 Matrix4::operator*(const Vector4& o) const
        {
            return Vector4(rows[0].x * o.x + rows[0].y * o.y + rows[0].z * o.z + rows[0].w * o.w,
                rows[1].x * o.x + rows[1].y * o.y + rows[1].z * o.z + rows[1].w * o.w,
                rows[2].x * o.x + rows[2].y * o.y + rows[2].z * o.z + rows[2].w * o.w,
                rows[3].x * o.x + rows[3].y * o.y + rows[3].z * o.z + rows[3].w * o.w);
        }


        Matrix4 Matrix4::operator*(float s) const
        {
            Matrix4 mat = *this;

            for (int i = 0; i < 16; ++i)
            {
                mat.array[i] *= s;
            }

            return mat;
        }

        Matrix4 Matrix4::Identity()
        {
            return Matrix4(1);
        }

        Matrix4 Matrix4::Scale(float x, float y, float z)
        {
            Vector4 r1(x, 0.f, 0.f, 0.f);
            Vector4 r2(0.f, y, 0.f, 0.f);
            Vector4 r3(0.f, 0.f, z, 0.f);
            Vector4 r4(0.f, 0.f, 0.f, 1.f);

            return Matrix4(r1, r2, r3, r4);
        }

        Matrix4 Matrix4::Translation(const Vector3& t)
        {
            Vector4 r1(1.f, 0.f, 0.f, 0.f);
            Vector4 r2(0.f, 1.f, 0.f, 0.f);
            Vector4 r3(0.f, 0.f, 1.f, 0.f);
            Vector4 r4(t.x, t.y, t.z, 1.f);

            return Matrix4(r1, r2, r3, r4);
        }

        Matrix4 Matrix4::RotationX(float a)
        {
            a = Rad(a);
            Vector4 r1(1.f, 0.f, 0.f, 0.f);
            Vector4 r2(0.f, Cos(a), -Sin(a), 0.f);
            Vector4 r3(0.f, Sin(a), Cos(a), 0.f);
            Vector4 r4(0.f, 0.f, 0.f, 1.f);

            return Matrix4(r1, r2, r3, r4);
        }

        Matrix4 Matrix4::RotationZ(float a)
        {
            a = Rad(a);
            Vector4 r1(Cos(a), -Sin(a), 0.f, 0.f);
            Vector4 r2(Sin(a), Cos(a), 0.f, 0.f);
            Vector4 r3(0.f, 0.f, 1.f, 0.f);
            Vector4 r4(0.f, 0.f, 0.f, 1.f);

            return Matrix4(r1, r2, r3, r4);
        }

        Matrix4 Matrix4::RotationY(float a)
        {
            a = Rad(a);
            Vector4 r1(Cos(a), 0.f, Sin(a), 0.f);
            Vector4 r2(0.f, 1.f, 0.f, 0.f);
            Vector4 r3(-Sin(a), 0.f, Cos(a), 0.f);
            Vector4 r4(0.f, 0.f, 0.f, 1.f);

            return Matrix4(r1, r2, r3, r4);
        }

        Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float znear, float zfar)
        {
            Matrix4 ortho(1);

			ortho(0, 0) = static_cast<float>(2) / (right - left);
			ortho(1, 1) = static_cast<float>(2) / (top - bottom);
			ortho(2, 2) = -static_cast<float>(2) / (zfar - znear);
			ortho(3, 0) = -(right + left) / (right - left);
			ortho(3, 1) = -(top + bottom) / (top - bottom);
			ortho(3, 2) = -(zfar + znear) / (zfar - znear);

			//ortho[0].x = -2.f / (right - left);
			//ortho[0].y = 0.f;
			//ortho[0].z = 0.f;
			//ortho[0].w = -(right + left) / (right - left);
			//	
			//ortho[1].x = 0.f;
			//ortho[1].y = 2.f / (top - bottom);
			//ortho[1].z = 0.f;
			//ortho[1].w = -(top + bottom) / (top - bottom);
			//	
			//ortho[2].x = 0.f;
			//ortho[2].y = 0.f;
			//ortho[2].z = 2.f / (zfar - znear);
			//ortho[2].w = -(zfar + znear) / (zfar - znear);
			//	
			//ortho[3].x = -1.f;
			//ortho[3].y = +1.f;
			//ortho[3].z = 0.f;
			//ortho[3].w = 1.f;

            return ortho;
        }

        Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
        {
            const Vector3 f = (center - eye).Normalize();
            const Vector3 s = f.Cross(up).Normalize();
            const Vector3 u = s.Cross(f);

            Matrix4 m = Matrix4::Identity();
            m(0, 0) = s.x;
            m(1, 0) = s.y;
            m(2, 0) = s.z;
            m(0, 1) = u.x;
            m(1, 1) = u.y;
            m(2, 1) = u.z;
            m(0, 2) = -f.x;
            m(1, 2) = -f.y;
            m(2, 2) = -f.z;
            m(3, 0) = -s.Dot(eye);
            m(3, 1) = -u.Dot(eye);
            m(3, 2) = f.Dot(eye);
            return m;
        }
    }
}