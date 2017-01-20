#pragma once

#include <Ace/Types.h>

namespace ace
{
    
    class Transform
    {
    public:

        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        Transform(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), const Vector3& scale = Vector3()) :
            position(position),
            rotation(rotation),
            scale(scale)
        {

        }



    };

}
