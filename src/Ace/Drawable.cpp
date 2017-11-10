#include <Ace/Drawable.h>
#include <Ace/GraphicsDevice.h>

#include <Ace/Log.h>
#include <Ace/Assert.h>
#include <Ace/Math.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <cstdio>

namespace ace
{
    char* TMXLiteFileCallback(const char* path)
    {
        File file(Path(path, true));
        UInt32 size = file.Size();

        UInt8* buffer = new UInt8[size + 1];
        file.Read(buffer, size);
        buffer[size] = '\0';

        return (char*)(buffer);
    }


    Drawable::~Drawable() { }

    struct Tilemap::TiledImpl
    {
        bool isMapLoaded;
        tmx::Map map;

        SpriteSheet sheet;
        Vector2 tilesetSize;

        std::vector<TileLayer> layers;

        TiledImpl(const Path path) : isMapLoaded(true)
        {
            std::string p = path.GetPath();

            if (!File::Exists(path))
            {
                Logger::LogError("Tilemap file doesn't exists: %s", p.c_str());
                ACE_ASSERT(0, "Tilemap file doesn't exists", "");
                return;
            }


            File file(path);
            UInt32 size = file.Size();
            const auto& buffer = file.ReadAllText();

            std::string workingDir = path.GetBasePath();

            tmx::fileCallback = TMXLiteFileCallback;

            if (!map.load(path.GetPath(), workingDir))
            {
                Logger::LogError("Tilemap failed load map: %s", p.c_str());
                ACE_ASSERT(0, "Tilemap failed load map", "");
                isMapLoaded = false;
            }
        }

        Texture GetTileset()
        {

            if (isMapLoaded && map.getTilesets().size() > 0)
            {
                std::string p = map.getTilesets()[0].getImagePath();
                Path path(p, true);
				Image image = Image::MissingFile();
	
                if (File::Exists(path))
                {
					image = Image(File(path));          
                }
				else
				{
                    Logger::LogError("Tilemap failed load image: %s", p.c_str());
                    ACE_ASSERT(0, "Tilemap failed load image", "");
				}
				
				sheet = SpriteSheet(image);

				char nameBuffer[8];
			
				sheet.AddSprite("NULL", Rect(0, 0, 0, 0)); // Null Sprite

				tilesetSize = math::MakeVektor<2u, float>(map.getTilesets()[0].getTileSize().x, map.getTilesets()[0].getTileSize().y);

				UInt32 col = map.getTilesets()[0].getColumnCount(), row = map.getTilesets()[0].getTileCount() / col;
				Rect location(0, 0, tilesetSize.x, tilesetSize.y);

				for (UInt32 y = 0; y < row; ++y)
				{
					for (UInt32 x = 0; x < col; ++x)
					{
						location.x = location.width * static_cast<Int32>(x);
						location.y = location.height * static_cast<Int32>(y);

						sprintf(nameBuffer, "%i", x + (col * y) + 1);
						sheet.AddSprite(nameBuffer, location);
					}
				}
				
				return Texture(sheet.image);
            }
			else
			{
				Logger::LogError("Tilemap failed load tiles");
			}

            return nullptr;
        }

        typedef Sprite(*SpriteBeginCallback)(Vector2& position, Vector2& size);
        typedef void(*SpriteEndCallback)(Sprite& sprite, Vector2& pivot);

        static Sprite BeginSprite(Vector2& position, Vector2& size)
        {
            return Sprite();
        }

        static void EndSprite(Sprite& sprite, Vector2& pivot)
        {

        }

        static Sprite BeginSpriteIsometric(Vector2& position, Vector2& size)
        {
            position = Isometric(position);
            //position.x /= -2.0f;
            return Sprite();
        }

        static void EndSpriteIsometric(Sprite& sprite, Vector2& pivot)
        {
            pivot -= Vector2(0.5f, 0.5f);
        }

        static Vector2 Isometric(const Vector2& point)
        {
            return Vector2(point.x - point.y, (point.x + point.y) / 2.0f);
        }

        static void GetSpriteCallback(const tmx::Orientation& orientation, SpriteBeginCallback& begin, SpriteEndCallback& end)
        {
            switch (orientation)
            {
            case tmx::Orientation::Isometric:
                begin = BeginSpriteIsometric;
                end = EndSpriteIsometric;
                break;

            default:
                begin = BeginSprite;
                end = EndSprite;
            }
        }

        void CreateLayers(float scale, const Vector3& pivot, ReadTilemap callback, void* arg)
        {
            if (!isMapLoaded)
            {
                return;
            }

            UInt32 col = map.getTileCount().x, row = map.getTileCount().y;
            tmx::Vector2u tileSize = map.getTileSize();

            SpriteBeginCallback begin = nullptr;
            SpriteEndCallback end = nullptr;
            SpriteSheet::SpriteData* spriteData = nullptr;

            GetSpriteCallback(map.getOrientation(), begin, end);

            // float offsetX = tileSize.x / tilesetSize.x, offsetY = tileSize.y / tilesetSize.y;

            const UInt32 layerCount = map.getLayers().size();
            for (UInt32 i = 0; i < layerCount; ++i)
            {
                TileLayer layer;
                tmx::TileLayer* tileLayer = dynamic_cast<tmx::TileLayer*>(map.getLayers()[i].get());

                if (tileLayer == nullptr)
                {
                    continue;
                }

                for (UInt32 y = 0; y < row; ++y)
                {
                    for (UInt32 x = 0; x < col; ++x)
                    {
                        const tmx::TileLayer::Tile& tile = tileLayer->getTiles()[x + y * col];

                        if (tile.ID == 0)
                        {
                            continue;   
                        }

                        Vector2 pos(static_cast<float>(x), static_cast<float>(y));
                        Vector2 size(static_cast<float>(tileSize.x), static_cast<float>(tileSize.y));

                        // Begin
                        Sprite sprite = begin(pos, size);
                        //pos.y *= (size.y / size.x);


                        // Update
                        // TODO: Flip

						sprite.SetID(tile.ID);


                        spriteData = sheet.GetSprite(tile.ID);
                        
                        if (spriteData != nullptr)
                        {
                            sprite.Texcoord(spriteData->texcoord);
                        }

                        sprite.Scale(Vector2(tilesetSize.x, tilesetSize.y) / math::Min(tileSize.x, tileSize.y));
                        sprite.Move(Vector3(pos.x, row-pos.y, i * pivot.z));

                        // End
                        // TODO: Fix pivot (offset)
                        Vector2 point(pivot.x, pivot.y);
                        end(sprite, point);
                        sprite.Move(Vector3(col * -point.x, row * -point.y, 0.f));

                        sprite.Scale(Vector2(scale, scale));

                        if (callback != nullptr)
                        {
                            callback(sprite, tile.ID, i, arg);
                        }

                        layer.tiles.push_back(sprite);
                    }
                }

				GraphicsDevice::CreateIndicies(layer.indexBuffer, layer.tiles.size(), BufferUsage::Static);
                layers.push_back(layer);
            }
        }
    };

    Tilemap::Tilemap(const Path& map, float scale, const Vector3& pivot, ReadTilemap callback, void* arg) : Drawable(), m_tiledImpl(new TiledImpl(map))
    {
        tileset = m_tiledImpl->GetTileset();
        m_tiledImpl->CreateLayers(scale, pivot, callback, arg);
    }

    Tilemap::~Tilemap()
    {

    }

    UInt32 Tilemap::LayersCount() const
    {
        return m_tiledImpl->layers.size();
    }

    Tilemap::TileLayer* Tilemap::GetLayer(UInt32 i)
    {
        return i < LayersCount() ? &m_tiledImpl->layers[i] : nullptr;
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
        for (UInt32 i = 0; i < LayersCount(); ++i)
        {
            GraphicsDevice::Draw(m_tiledImpl->layers[i]);
        }
    }

    void Tilemap::TileLayer::Draw() const
    {
		//while(1)
		//	Logger::LogError("Tilemap drawing... %i", tiles.size());
		
		GraphicsDevice::Draw(tiles.data(), tiles.size(), indexBuffer);

        //for (UInt32 i = 0; i < tiles.size(); ++i)
        //{
        //    // TODO: Draw all sprites. (single draw call)
        //    GraphicsDevice::Draw(tiles[i]);
        //}
    }

    tmx::Map& Tilemap::GetMap()
    {
        return m_tiledImpl->map;
    }

    Tilemap::operator bool() const
    {
        return m_tiledImpl && m_tiledImpl->isMapLoaded;
    }
}
