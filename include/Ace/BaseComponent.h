#pragma once

namespace ace
{

    class Entity;
    class Transform;

    struct BaseComponent
    {
        BaseComponent(const Entity& entity);

        const Entity* entity;
        const Transform* transform;
    };
}