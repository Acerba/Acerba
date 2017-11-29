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

        TiledImpl() : isMapLoaded(false)
        {

        }

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
            else
            {
                col = map.getTileCount().x;
                row = map.getTileCount().y;

                mapWidth = map.getTileSize().x;
                mapHeight = map.getTileSize().y;
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

        Vector2 GetOffset(const Vector2& offset)
        {
            float w = mapWidth / static_cast<float>(mapHeight);
            float h = mapHeight / static_cast<float>(mapWidth);

            if (w > h)
            {
                h = 1.0f;
            }

            Vector2 result = Vector2(col * offset.x, row * offset.y) * scale;

            if (map.getOrientation() == tmx::Orientation::Isometric)
            {
                return Isometric(result);
            }

            return result;
        }

        Vector3 GetOffset(const Vector3& offset)
        {
            Vector2 getOffset = GetOffset(Vector2(offset.x, offset.y));
            return Vector3(getOffset.x, getOffset.y, offset.z);
        }

        void CreateLayers(const Vector3& offset, ReadTilemap callback, void* arg)
        {
            if (!isMapLoaded)
            {
                return;
            }

            SpriteBeginCallback begin = nullptr;
            SpriteEndCallback end = nullptr;
            
            GetSpriteCallback(map.getOrientation(), begin, end);

            // float offsetX = tileSize.x / tilesetSize.x, offsetY = tileSize.y / tilesetSize.y;

            tmx::Layer* layerImpl;

            Vector3 mapOffset = GetOffset(offset);

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
                            Vector2 size(static_cast<float>(mapWidth), static_cast<float>(mapHeight));

                            // Begin
                            Sprite sprite = begin(pos, size);
                            //pos.y *= (size.y / size.x);

                            // Update
                            // TODO: Flip

                            sprite.SetID(tile.ID);
                            sprite.Texcoord(sheet.GetSprite(tile.ID)->texcoord);
                            sprite.Scale(Vector2(tilesetSize.x, tilesetSize.y) / math::Min(mapWidth, mapHeight));

                            sprite.Move(Vector3(pos.x, row - pos.y, i * pivot.z));

                            // End
                            // TODO: Fix pivot (offset)
                            Vector2 point(pivot.x, pivot.y);
                            end(sprite, point);
                            sprite.Move(Vector3(col * -point.x, row * -point.y, 0.f));
                            sprite.Scale(Vector2(scale, scale));

                            sprite.Move(mapOffset);

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


    // Tilemap //

    Tilemap::Tilemap() : Drawable(), m_tiledImpl(new TiledImpl())
    {

    }

    Tilemap::Tilemap(const Path& map, float scale, const Vector3& pivot) : Drawable(), m_tiledImpl(new TiledImpl(map))
    {
        tileset = m_tiledImpl->GetTileset();
        m_tiledImpl->pivot = pivot;
        m_tiledImpl->scale = scale;
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
        //#if ACE_DEBUG
        //Sprite sprite;
        //sprite.Scale(Vector2(0.01f, 0.01f));
        //StandardMaterial mat;
        //mat->color = Color32(1.0f);
        //GraphicsDevice::SetMaterial(mat);
        //
        //for (UInt32 i = 0; i < m_tiledImpl->collision.size(); ++i)
        //{
        //    const auto vertices(m_tiledImpl->collision[i]->GetVertices());
        //    const UInt32 size = vertices.size();
        //    for (UInt32 j = 0; j < size; ++j)
        //    {
        //        mat->position = vertices[j];
        //        GraphicsDevice::Draw(sprite);
        //    }
        //}
        //#endif
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

		iso = TiledImpl::Isometric(iso);
		iso.y = m_tiledImpl->row - iso.y;

		Vector3 pivot = m_tiledImpl->pivot - Vector3(0.5f, 0.0f, 0.0f);

		iso -= Vector2(m_tiledImpl->col * -pivot.x, m_tiledImpl->row * pivot.y);

		return iso * m_tiledImpl->scale;
	}

    void Tilemap::CreateTiles(const Vector2& offset, float depth, ReadTilemap callback, void* arg)
    {
        m_tiledImpl->CreateLayers(Vector3(offset.x, offset.y, depth), callback, arg);
    }

	bool Tilemap::CreateCollisions(UInt32 layer, const Vector2& offset)
	{
		if (m_tiledImpl->objects.size() > layer)
		{
			tmx::ObjectGroup* objectLayer = m_tiledImpl->objects[layer];

			Vector2 pos;

            Vector2 mapOffset = m_tiledImpl->GetOffset(offset);

			for (UInt32 i = 0; i < objectLayer->getObjects().size(); ++i)
			{
				if (objectLayer->getObjects()[i].getShape() == tmx::Object::Shape::Polygon)
				{
					pos.x = objectLayer->getObjects()[i].getPosition().x;
					pos.y = objectLayer->getObjects()[i].getPosition().y;

                    const tmx::Object& obj = objectLayer->getObjects()[i];
                    const auto& points = obj.getPoints();

                    if (points.size() == 3)
                    {
                        m_tiledImpl->collision.emplace_back(std::make_unique<Triangle>(
                            GetPosition(pos, Vector2(points[0].x, points[0].y)) + mapOffset,
                            GetPosition(pos, Vector2(points[1].x, points[1].y)) + mapOffset,
                            GetPosition(pos, Vector2(points[2].x, points[2].y)) + mapOffset
                        ));
                    }

					else if (points.size() == 4)
					{
                        m_tiledImpl->collision.emplace_back(std::make_unique<Rectangle>(
                            GetPosition(pos, Vector2(points[0].x, points[0].y)) + mapOffset,
                            GetPosition(pos, Vector2(points[1].x, points[1].y)) + mapOffset,
                            GetPosition(pos, Vector2(points[2].x, points[2].y)) + mapOffset,
                            GetPosition(pos, Vector2(points[3].x, points[3].y)) + mapOffset
                        ));
					}
				}
			}

            return true;
		}

		return false;
	}
	
	bool Tilemap::CreateCollisions(std::string layer, const Vector2& offset)
	{
		for (UInt32 i = 0; i < m_tiledImpl->objects.size(); ++i)
		{
			if (m_tiledImpl->objects[i]->getName() == layer)
			{
				return CreateCollisions(layer, offset);
			}
		}

		return false;
	}


    void Tilemap::Append(const Tilemap& map, const Vector2& offset, float depth)
    {
        Vector2 mapOffset = map.m_tiledImpl->GetOffset(offset);
        Vector3 tielOffset(mapOffset.x, mapOffset.y, depth);

        UInt32 layersCount = m_tiledImpl->layers.size();
        UInt32 collisionsCount = m_tiledImpl->collision.size();

        m_tiledImpl->layers.insert(m_tiledImpl->layers.end(), map.m_tiledImpl->layers.begin(), map.m_tiledImpl->layers.end());

        for (UInt32 i = 0; i < map.m_tiledImpl->collision.size(); ++i)
        {
            std::vector<Vector2> vertices = map.m_tiledImpl->collision[i]->GetVertices();

            if (vertices.size() == 3)
            {
                m_tiledImpl->collision.emplace_back(std::make_unique<Triangle>(
                    vertices[0] + mapOffset,
                    vertices[1] + mapOffset,
                    vertices[2] + mapOffset
                 ));
            }
            else if (vertices.size() == 4)
            {
                m_tiledImpl->collision.emplace_back(std::make_unique<Rectangle>(
                    vertices[0] + mapOffset,
                    vertices[1] + mapOffset,
                    vertices[2] + mapOffset,
                    vertices[3] + mapOffset
                ));
            }
        }

        if (offset.x != 0 || offset.y != 0)
        {
            for (UInt32 layer = layersCount; layer < m_tiledImpl->layers.size(); ++layer)
            {
                for (UInt32 i = 0; i < m_tiledImpl->layers[layer].tiles.size(); ++i)
                {
                    m_tiledImpl->layers[layer].tiles[i].Move(tielOffset);
                }
            }
        }

        if (!m_tiledImpl->isMapLoaded)
        {
            m_tiledImpl->col = map.m_tiledImpl->col;
            m_tiledImpl->row = map.m_tiledImpl->row;
            m_tiledImpl->scale = map.m_tiledImpl->scale;
            
            m_tiledImpl->pivot = map.m_tiledImpl->pivot;
            m_tiledImpl->tileHeight = map.m_tiledImpl->tileHeight;
            m_tiledImpl->tileWidth = map.m_tiledImpl->tileWidth;
            m_tiledImpl->tilesetSize = map.m_tiledImpl->tilesetSize;

            m_tiledImpl->sheet = map.m_tiledImpl->sheet;
            m_tiledImpl->isMapLoaded = map.m_tiledImpl->isMapLoaded;

            tileset = map.tileset;
        }
    }

    UInt32 Tilemap::GetRows() const
    {
        return m_tiledImpl->row;
    }

    UInt32 Tilemap::GetCols() const
    {
        return m_tiledImpl->col;
    }

    UInt32 Tilemap::GetWidth() const
    {
        return m_tiledImpl->mapWidth;
    }

    UInt32 Tilemap::GeHeight() const
    {
        return m_tiledImpl->mapHeight;
    }
}
