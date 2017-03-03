#pragma once

#include <Ace/Macros.h>

namespace ace
{
    class Material;
    class Scene;
    class SpriteManagerImpl;

    class SpriteManager
    {

        SpriteManagerImpl* m_impl;

        SpriteManager();

        ~SpriteManager();

        static SpriteManager& GetInstance();

        ACE_DISABLE_COPY(SpriteManager)

    public:

        /**
        @brief Draw all entities which are parented to scene and have both Material and Sprite attached to them.
        @param[in] scene Target scene whose children to draw.
        @param[in] material Pointer to a material to use instead of the entities own materials. Uses entities materials by default.
        */
        static void Draw(const Scene& scene, Material* material = nullptr);

    };



}