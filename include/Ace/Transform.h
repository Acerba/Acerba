#pragma once

#include <Ace/Types.h>

namespace ace
{

    class Transform
    {
    public:

        Matrix4 model;
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;


        /**
        @brief Transform class, XYZ-coodrinates
        @param[in] position Position, default 0,0,0
        @param[in] rotation Rotation, default 0,0,0,0
        @param[in] scale Scale, default 1,1,1
        */
        Transform(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), const Vector3& scale = Vector3(1.f, 1.f, 1.f)) :
            position(position),
            rotation(rotation),
            scale(scale)
        {

        }



    };

}
