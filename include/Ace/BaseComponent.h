#pragma once

#include <Ace/EntityHandle.h>

namespace ace
{
    class Transform;
    class Entity;

    struct BaseComponent
    {
        BaseComponent(Entity& entity);

        EntityHandle* const entity;
        Transform* const  transform;
    };
}