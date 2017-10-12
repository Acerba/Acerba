#include <Ace/Drawable.h>
#include <Ace/GraphicsDevice.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <cstdio>

namespace ace
{
    struct Tilemap::TiledImpl
    {
        tmx::Map map;

        SpriteSheet sheet;
        std::vector<TileLayer> layers;

        TiledImpl(const Path file)
        {
            if (!map.load(file))
            {
                // Error
            }
        }

        Texture GetTileset()
        {
            if (map.getTilesets().size() > 0)
            {
                Path path(map.getTilesets()[0].getImagePath(), true);

                if (File::Exists(path))
                {
                    char nameBuffer[8];

                    sheet = SpriteSheet(Image(File(path)));
                    sheet.AddSprite("NULL", Rect(0,0,0,0)); // Null Sprite



                    UInt32 col = map.getTilesets()[0].getColumnCount(), row = map.getTilesets()[0].getTileCount() / col;
                    Rect location(0, 0, map.getTilesets()[0].getTileSize().x, map.getTilesets()[0].getTileSize().y);
                    
                    for (Int32 y = 0; y < row; ++y)
                    {
                        for (Int32 x = 0; x < col; ++x)
                        {
                            location.x = location.width * x;
                            location.y = location.height * y;

                            sprintf_s(nameBuffer, "%i", x + col * y + 1);
                            sheet.AddSprite(nameBuffer, location);
                        }
                    }

                    return Texture(sheet.image);
                }

            }

            return nullptr;
        }

        void CreateLayers()
        {

            UInt32 col = map.getTileCount().x, row = map.getTileCount().y;
            tmx::Vector2u tileSize = map.getTileSize();

            for (Int32 i = 0; i < map.getLayers().size(); ++i)
            {
                TileLayer layer;
                tmx::TileLayer* tileLayer = dynamic_cast<tmx::TileLayer*>(map.getLayers()[i].get());
                
                if (tileLayer == nullptr)
                {
                    continue;
                }

                const float SCALE = 10.0f;

                for (Int32 y = 0; y < row; ++y)
                {
                    for (Int32 x = 0; x < col; ++x)
                    {
                        Sprite sprite;
                        const tmx::TileLayer::Tile& tile = tileLayer->getTiles()[x + y * col];

                        // TODO: Position (Offset)
                        if (tile.ID == 0)
                            continue;
                        
                        // Texcoord or SetSprite...
                        // TODO: Scale
                        sprite.Texcoord(sheet.GetSprite(tile.ID)->texcoord);
                        //sprite.SetSprite(sheet.GetSprite(tile.ID), 10.0f);


                        // TODO: Flip

                        sprite.Move(Vector3(x, (row-y), 0));
                        layer.tiles.push_back(sprite);
                    }
                }

  
                layers.push_back(layer);
            }
        }
    };

    Tilemap::Tilemap(const Path& map) : Drawable(), m_tiledImpl(new TiledImpl(map))
    {
        tileset = m_tiledImpl->GetTileset();
        m_tiledImpl->CreateLayers();
    }

    Tilemap::~Tilemap()
    {

    }

    UInt32 Tilemap::LayersCount() const
    {
        return m_tiledImpl->layers.size();
    }

    Tilemap::TileLayer* Tilemap::GetLayer(Int32 i)
    {
        if (i > LayersCount())
        {
            return nullptr;
        }

        return &m_tiledImpl->layers[i];
    }

    SpriteSheet& Tilemap::GetSpriteSheet()
    {
        return m_tiledImpl->sheet;
    }

    const SpriteSheet& Tilemap::GetSpriteSheet() const
    {
        return m_tiledImpl->sheet;
    }
    
    void Tilemap::Draw() const
    {
        for (Int32 i = 0; i < LayersCount(); ++i)
        {
            GraphicsDevice::Draw(m_tiledImpl->layers[i]);
        }
    }

    void Tilemap::TileLayer::Draw() const
    {
        for (Int32 i = 0; i < tiles.size(); ++i)
        {
            // TODO: Draw all sprites. (single draw call)
            GraphicsDevice::Draw(tiles[i]);
        }
    }
}
