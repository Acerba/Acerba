#pragma once

#include <Ace/Platform.h>
#include <Ace/Types.h>

#if ACE_DEBUG
    #include <Ace/Log.h>
#endif

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
            model(Matrix4::Identity()),
            position(position),
            rotation(rotation),
            scale(scale)
        {

        }

        #if ACE_DEBUG
            void Log(const char* msg = nullptr) const
            {
                Logger::LogDebug(msg);
                model.Log("model\n");
                position.Log("position\n");
                rotation.Log("rotation\n");
                scale.Log("scale\n");
            }
        #endif

    };
}
