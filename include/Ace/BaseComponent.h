#pragma once

#include <Ace/EntityHandle.h>

namespace ace
{
    class Transform;
    class Entity;

    struct BaseComponent
    {
        EntityHandle* const entity;
        Transform* const transform;

        BaseComponent(Entity& entity);

        void operator=(const BaseComponent& base);

    };
}