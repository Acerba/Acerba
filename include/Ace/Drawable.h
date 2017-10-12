#pragma once
 
#include <Ace/Vector2.h>

#include <Ace/Sprite.h>
#include <Ace/SpriteSheet.h>

#include <Ace/Path.h>

#include <memory>
#include <vector>

namespace ace
{
    struct Drawable
    {
        /**
            @brief Custom Draw method
            Requires manual buffer or sprite logic.
        */
        virtual void Draw() const = 0;
    };


    class Tilemap : public Drawable
    {
    public:

        struct TileLayer : public Drawable
        {
            std::vector<Sprite> tiles;

            virtual void Draw() const;
        };

    private:

        struct TiledImpl;
        std::shared_ptr<TiledImpl> m_tiledImpl;

    public:

        Texture tileset;

        Tilemap(const Path& map);
        ~Tilemap();

        UInt32 LayersCount() const;
        TileLayer* GetLayer(Int32 i);

        SpriteSheet& GetSpriteSheet();
        const SpriteSheet& GetSpriteSheet() const;

        virtual void Draw() const;
    };
}