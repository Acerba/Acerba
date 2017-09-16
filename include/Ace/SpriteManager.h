#pragma once

#include <Ace/Buffer.h>
#include <Ace/EntityManager.h>
#include <Ace/Macros.h>
#include <Ace/Material.h>
#include <Ace/Scene.h>
#include <Ace/Sprite.h>

#include <vector>

namespace ace
{

    class Camera;

    class SpriteManager
    {
        struct Group
        {
            Material material;
            UInt32 start;
            UInt32 end;

            Group(const Material& mat, const UInt32 begin = static_cast<UInt32>(-1));
        };


        std::vector<Sprite> m_sprites;
        Buffer m_buffer;
        UInt32* m_indexTable;
        UInt32 m_size;


        SpriteManager();

        ~SpriteManager();

        void DrawImpl(const Scene& scene, const Camera& camera, const Material* material);

        static SpriteManager& GetInstance();

        void HandleIndices(UInt32 newSize);

        std::vector<Group> Sort(const Scene& scene);

        std::vector<UInt32> SortIndices(const std::vector<EntityManager::EntityHandle*>& handles) const;


        ACE_DISABLE_COPY(SpriteManager)

    public:

        /**
        @brief Draw all entities which are parented to scene and have both Material and Sprite attached to them.
        @param[in] scene Target scene whose children to draw.
        @param[in] material Pointer to a material to use instead of the entities own materials. Uses entities materials by default.
        */
        static void Draw(const Scene& scene, const Camera& camera, const Material* material = nullptr);

    };



}