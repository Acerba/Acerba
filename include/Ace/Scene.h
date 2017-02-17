#pragma once

#include <Ace/Macros.h>

namespace ace
{
    class Entity;
    class EntityManager;

    class Scene
    {

        Entity* m_root;

        ACE_DISABLE_COPY(Scene)

    public:

        Scene(EntityManager* rootManager = nullptr);
        ~Scene();

        const Entity* GetRoot() const;
        Entity* GetRoot();



    };
}