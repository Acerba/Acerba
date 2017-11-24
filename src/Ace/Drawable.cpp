#include <Ace/Drawable.h>
#include <Ace/GraphicsDevice.h>

#include <Ace/Log.h>
#include <Ace/Assert.h>
#include <Ace/Math.h>

#include <Ace/Collidable.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <cstdio>
#include <memory>

#include <Ace/StandardMaterial.h>

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

		UInt32 mapWidth, mapHeight;
		UInt32 tileWidth, tileHeight;
		UInt32 row, col;

		Vector3 pivot;
		float scale;

        std::vector<TileLayer> layers;
        std::vector<tmx::ObjectGroup*> objects;

		std::vector< std::unique_ptr<Collidable> > collision;

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
				tileWidth = tilesetSize.x;
				tileHeight = tilesetSize.y;


				UInt32 col = map.getTilesets()[0].getColumnCount();
				UInt32 row = map.getTilesets()[0].getTileCount() / col;
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
				
                return sheet.spriteSheet;
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
            pivot -= Vector2(0.5f, 0.0f);
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

        void CreateLayers(float scal, const Vector3& piv, ReadTilemap callback, void* arg)
        {
            if (!isMapLoaded)
            {
                return;
            }

			scale = scal;
			pivot = piv;

			col = map.getTileCount().x;
			row = map.getTileCount().y;

            tmx::Vector2u tileSize = map.getTileSize();
			mapWidth = tileSize.x;
			mapHeight = tileSize.y;

            SpriteBeginCallback begin = nullptr;
            SpriteEndCallback end = nullptr;

            GetSpriteCallback(map.getOrientation(), begin, end);

            // float offsetX = tileSize.x / tilesetSize.x, offsetY = tileSize.y / tilesetSize.y;

            tmx::Layer* layerImpl;

            const UInt32 layerCount = map.getLayers().size();
            for (UInt32 i = 0; i < layerCount; ++i)
            {
                TileLayer layer;

                layerImpl = map.getLayers()[i].get();

                if (layerImpl->getType() == tmx::Layer::Type::Object)
                {
                    objects.push_back(static_cast<tmx::ObjectGroup*>(map.getLayers()[i].get()));
                }
                else if (layerImpl->getType() == tmx::Layer::Type::Tile)
                {
                    tmx::TileLayer* tileLayer = static_cast<tmx::TileLayer*>(map.getLayers()[i].get());

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
                            sprite.Texcoord(sheet.GetSprite(tile.ID)->texcoord);
                            sprite.Scale(Vector2(tilesetSize.x, tilesetSize.y) / math::Min(tileSize.x, tileSize.y));

                            sprite.Move(Vector3(pos.x, row - pos.y, i * pivot.z));

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
        }

        void Move(const Vector2& offset)
        {
            for (UInt32 i = 0; i < layers.size(); ++i)
            {
                for (UInt32 j = 0; j < layers[i].tiles.size(); ++j)
                {
                    layers[i].tiles[j].Move(offset);
                }
            }

            for (UInt32 i = 0; i < collision.size(); ++i)
            {
                Vector2& pos = collision[i]->GetLocalPosition();
                pos += offset;
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

    void Tilemap::Move(const Vector2& offset)
    {
        m_tiledImpl->Move(offset);
    }

    void Tilemap::Draw() const
    {
        for (UInt32 i = 0; i < LayersCount(); ++i)
        {
            GraphicsDevice::Draw(m_tiledImpl->layers[i]);
        }

        // Debug Draw
        #if 1
        Sprite sprite;
        sprite.Scale(Vector2(0.01f, 0.01f));
        StandardMaterial mat;
        mat->color = Color32(1.0f);
        GraphicsDevice::SetMaterial(mat);

        for (UInt32 i = 0; i < m_tiledImpl->collision.size(); ++i)
        {
            for (UInt32 j = 0; j < m_tiledImpl->collision[i]->GetVertices().size(); ++j)
            {
                mat->position = m_tiledImpl->collision[i]->GetVertices()[j];
                GraphicsDevice::Draw(sprite);
            }
        }
        #endif
    }

    void Tilemap::TileLayer::Draw() const
    {	
		GraphicsDevice::Draw(tiles.data(), tiles.size(), indexBuffer);
    }

    tmx::Map& Tilemap::GetMap()
    {
        return m_tiledImpl->map;
    }

    std::vector<tmx::ObjectGroup*>& Tilemap::GetObjects()
    {
        return m_tiledImpl->objects;
    }

    Tilemap::operator bool() const
    {
        return m_tiledImpl && m_tiledImpl->isMapLoaded;
    }

	Vector2 Tilemap::GetPosition(float x, float y, const Vector2& offset) const
	{
		return GetPosition(Vector2(x, y), offset);
	}

	Vector2 Tilemap::GetPosition(const Vector2& position, const Vector2& offset) const
	{
		// TODO: Orthogonal position.

		Vector2 iso = position + offset;
        float min = math::Min(m_tiledImpl->mapWidth, m_tiledImpl->mapHeight);
		iso.x /= min;
		iso.y /= min;

		//iso.x = math::Round(iso.x);
		//iso.y = math::Round(iso.y);

		iso = TiledImpl::Isometric(iso);
		iso.y = m_tiledImpl->row - iso.y;

		Vector3 pivot = m_tiledImpl->pivot - Vector3(0.5f, 0.0f, 0.0f);

		iso -= Vector2(m_tiledImpl->col * -pivot.x, m_tiledImpl->row * pivot.y);

		return iso * m_tiledImpl->scale;
	}

	bool Tilemap::CreateCollisions(UInt32 layer)
	{
		if (m_tiledImpl->objects.size() > layer)
		{
			tmx::ObjectGroup* objectLayer = m_tiledImpl->objects[layer];

			Vector2 pos;

			for (UInt32 i = 0; i < objectLayer->getObjects().size(); ++i)
			{
				if (objectLayer->getObjects()[i].getShape() == tmx::Object::Shape::Polygon)
				{
					pos.x = objectLayer->getObjects()[i].getPosition().x;
					pos.y = objectLayer->getObjects()[i].getPosition().y;

                    const tmx::Object& obj = objectLayer->getObjects()[i];

                    if (obj.getPoints().size() == 3)
                    {
                        Vector2 triangle1[3] =
                        {
                            GetPosition(pos, Vector2(obj.getPoints()[0].x, obj.getPoints()[0].y)) - pos,
                            GetPosition(pos, Vector2(obj.getPoints()[1].x, obj.getPoints()[1].y)) - pos,
                            GetPosition(pos, Vector2(obj.getPoints()[2].x, obj.getPoints()[2].y)) - pos,
                        };

                        m_tiledImpl->collision.push_back(std::make_unique<Triangle>(
                            triangle1,
                            pos
                        ));
                    }

					if (obj.getPoints().size() == 4)
					{                        
                        m_tiledImpl->collision.push_back(std::make_unique<Rectangle>(
                            GetPosition(pos, Vector2(obj.getPoints()[0].x, obj.getPoints()[0].y)),
                            GetPosition(pos, Vector2(obj.getPoints()[1].x, obj.getPoints()[1].y)),
                            GetPosition(pos, Vector2(obj.getPoints()[2].x, obj.getPoints()[2].y)),
                            GetPosition(pos, Vector2(obj.getPoints()[3].x, obj.getPoints()[3].y))
                        ));

					}
				}
			}

		}

		return false;
	}
	
	bool Tilemap::CreateCollisions(std::string layer)
	{
		for (UInt32 i = 0; i < m_tiledImpl->objects.size(); ++i)
		{
			if (m_tiledImpl->objects[i]->getName() == layer)
			{
				return CreateCollisions(layer);
			}
		}

		return false;
	}
}
