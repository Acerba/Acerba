#pragma once

#include <Ace/Vector4.h>
#include <Ace/Vector3.h>
#include <Ace/IntTypes.h>


namespace ace
{
    namespace math
    {

        class Matrix4 final
        {
        public:

            union
            {
                struct
                {
                    Vector4 rows[4];
                };

                float array[16];

                // Rows, Columns
                float data[4][4];

            };

            Matrix4();
            Matrix4(float identity);
            Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);

            /**
            @return Determinant
            */
            float Determinant() const;
            /**
            @return Transpose matrix
            */
            Matrix4 Transpose() const;
            /**
            @return Inverse marix
            */
            Matrix4 Inverse() const;
            /**
            @return Cofactor matrix
            */
            Matrix4 Cofactor() const;
            /**
            @return Adjunct matrix
            */
            Matrix4 Adjunct() const;
            Matrix4 operator*(const Matrix4& m) const;
            Vector4 operator*(const Vector4& o) const;
            Matrix4 operator*(float scalar) const;

            /**
            @return Identity matrix
            */
            static Matrix4 Identity();
            /**
            @param[in] x Scaling for x-axis
            @param[in] y Scaling for y-axis
            @param[in] z Scaling for z-axis
            @return Scale matrix
            */
            static Matrix4 Scale(float x, float y, float z);
            /**
            @param[in] Vector3
            @return Translation matrix
            */
            static Matrix4 Translation(const Vector3& t);
            /**
            @param[in] a Rotation in degrees
            @return RotationX matrix
            */
            static Matrix4 RotationX(float a);
            /**
            @param[in] a Rotation in degrees
            @return RotationY matrix
            */
            static Matrix4 RotationY(float a);
            /**
            @param[in] a Rotation in degrees
            @return RotationZ matrix
            */
            static Matrix4 RotationZ(float a);
            /**
            @param[in] left Set left boundary
            @param[in] right Set right boundary
            @param[in] bottom Set bottom boundary
            @param[in] top Set top boundary
            @param[in] znear Set depth near boundary
            @param[in] zfar Set depth far boundary
            @return Orthographic matrix
            */
            static Matrix4 Ortho(float left, float right, float bottom, float top, float znear, float zfar);
            /**
            @param[in] eye Location of camera's viewpoint
            @param[in] center Dirction where camera is looking
            @param[in] up Vector defining upward direction

            @return LookAt matrix
            */
            static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);


            Vector4& operator[](UInt8 index)
            {
                return rows[index];
            }

            Vector4 operator[](UInt8 index)const
            {
                return rows[index];
            }

            float& operator()(UInt8 r, UInt8 c)
            {
                return data[r][c];
            }

            float operator()(UInt8 r, UInt8 c)const
            {
                return data[r][c];
            }

        };

    }
}