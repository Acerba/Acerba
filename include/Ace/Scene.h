#pragma once

#include <Ace/Macros.h>

namespace ace
{
    class Entity;

    class Scene
    {
        
        Entity* m_root;

        ACE_DISABLE_COPY(Scene)

    public:

        Scene();
        ~Scene();


    };
}