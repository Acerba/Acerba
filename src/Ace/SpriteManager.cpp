#include <Ace/SpriteManager.h>

#include <Ace/GraphicsDevice.h>
#include <Ace/Sprite.h>

#include <Ace/Component.h>
#include <Ace/ComponentPool.h>
#include <Ace/EntityHandle.h>
#include <Ace/Scene.h>
#include <Ace/Transform.h>

#include <algorithm>
#include <iterator>
#include <vector>

namespace ace
{

    struct Group
    {
        Material material;
        UInt32 start;
        UInt32 end;

        Group(const Material& mat) :
            material(mat),
            start(-1),
            end(0)
        {

        }
    };


    template <typename FWIter, typename Type>
    void ACE_iota(FWIter first, FWIter last, Type value)
    {
        while (first != last)
        {
            *first++ = value;
            ++value;
        }
    }

	// REMOVEME: CTRL+A & DELETE
    class SpriteManagerImpl
    {

        std::vector<Sprite> m_sprites;
        Buffer m_buffer;
        UInt32* m_indexTable;
        UInt32 m_size;

        std::vector<UInt32> _sortIndices(const std::vector<EntityManager::EntityHandle*>& handles) const
        {
            std::vector<UInt32> indices(m_sprites.size());
            ACE_iota(indices.begin(), indices.end(), 0u);

            std::sort(indices.begin(), indices.end(),
                [&handles](const UInt32 i1, const UInt32 i2){return handles[i1]->m_transform->position.z > handles[i2]->m_transform->position.z; }
            );

            return indices;
        }

        std::vector<Group> _sort()
        {
            //Temp storage
            std::vector<EntityManager::EntityHandle*> handles;
            std::vector<Sprite> sprites;
            std::vector<Group> groups;
            EntityManager::ComponentPool<Material>& primaryPool = EntityManager::ComponentPool<Material>::GetPool();
            EntityManager::ComponentPool<Sprite>& secondaryPool = EntityManager::ComponentPool<Sprite>::GetPool();
            EntityManager::ComponentBaseHandle* secondary = nullptr;

            //Find all entities that have both material and sprite
            for (UInt32 i = 0u; i < primaryPool.m_components.size(); ++i)
            {
                if (primaryPool.m_handles[i]->entity->Count() > 1u && (secondary = primaryPool.m_handles[i]->entity->Get<Sprite>()) != nullptr)
                {
                    bool added = false;

					// FIXME:
                    //Group size grows
                    for (auto& itr : groups)
                    {
                        if (itr.material.impl == primaryPool.m_components[i].impl)
                        {
                            ++itr.start;
                            ++itr.end;
                            added = true;
                            break;
                        }
                    }
                    //New material, new group
                    if (added == false)
                    {
                        groups.emplace_back(primaryPool.m_components[i]);
                    }

                    //Temporarily store sprite and its handle
                    sprites.emplace_back(secondaryPool.m_components[secondary->index]);
                    handles.emplace_back(secondaryPool.m_handles[secondary->index]->entity);
                }
            }

			// FIXME:
			groups[0].start = 0;
			groups[0].end = 2;

            m_sprites.reserve(sprites.size());

			// FIXME:
            //Sort spriteindices by Z-value and create sorted m_sprites
			//for (const auto& itr : _sortIndices(handles))
			//{
            //    m_sprites.emplace_back(sprites[itr]);
            //}

			m_sprites.insert(m_sprites.begin(), sprites.begin(), sprites.end());

            return groups;
        }


        void _handleIndices(const UInt32 newSize)
        {
            if (!m_indexTable)
            {
                m_size = newSize;
                m_indexTable = new UInt32[newSize * 6];
                for (UInt32 j = 0u; j < newSize; ++j)
                {
                    m_indexTable[0u + 6u * j] = 0u + 6u * j;
                    m_indexTable[1u + 6u * j] = 1u + 6u * j;
                    m_indexTable[2u + 6u * j] = 2u + 6u * j;
                    m_indexTable[3u + 6u * j] = 0u + 6u * j;
                    m_indexTable[4u + 6u * j] = 2u + 6u * j;
                    m_indexTable[5u + 6u * j] = 3u + 6u * j;
                }
            }
            else if (newSize > m_size)
            {
                UInt32* temp = new UInt32[m_size * 6];
                std::copy(m_indexTable, m_indexTable + (m_size - 1u), temp);
                delete[]m_indexTable;
                m_indexTable = new UInt32[newSize * 6];
                std::copy(temp, temp + (m_size - 1u), m_indexTable);

                for (UInt32 j = m_size; j < newSize; ++j)
                {
                    m_indexTable[0u + 6u * j] = 0u + 6u * j;
                    m_indexTable[1u + 6u * j] = 1u + 6u * j;
                    m_indexTable[2u + 6u * j] = 2u + 6u * j;
                    m_indexTable[3u + 6u * j] = 0u + 6u * j;
                    m_indexTable[4u + 6u * j] = 2u + 6u * j;
                    m_indexTable[5u + 6u * j] = 3u + 6u * j;
                }

                m_size = newSize;
            }
        }

    public:

        void Draw(const Scene& scene, Material* material)
        {
            std::vector<Group> groups(_sort());

            //Checks and grows m_indexTable if needed
			// FIXME:
            _handleIndices(std::distance(groups.begin(), std::max_element(groups.begin(), groups.end(),
                [](const Group& g1, const Group& g2){return g1.end - g1.start < g2.end - g2.start; })));

            //TODO: Change loop and both Draw-functions params to const if GraphicsDevice::Draw material accepts const
            for (auto& itr : groups)
            {
                const UInt32 indexCount = itr.end - itr.start;
                GraphicsDevice::BufferData(m_buffer, indexCount*4, m_sprites[itr.start].vertexData.data(), BufferUsage::Streaming);
                GraphicsDevice::SetBuffer(m_buffer);
				GraphicsDevice::Draw(material ? *material : itr.material, 0u, indexCount * 6u, m_indexTable);
            }

            //TODO: Add logic to check if the sprites may be saved
            m_sprites.clear();
        }

        SpriteManagerImpl() :
            m_sprites(),
            m_buffer(GraphicsDevice::CreateBuffer(BufferType::Vertex)),
            m_indexTable(nullptr),
            m_size(0u)
        {

		}

        ~SpriteManagerImpl()
        {
            if (m_indexTable)
            {
                delete[]m_indexTable;
                m_indexTable = nullptr;
            }
        }


    };




    SpriteManager::SpriteManager() :
        m_impl(new SpriteManagerImpl())
    {

    }


    SpriteManager::~SpriteManager()
    {
        if (m_impl)
        {
            delete m_impl;
            m_impl = nullptr;
        }
    }


    SpriteManager& SpriteManager::GetInstance()
    {
        static SpriteManager instance;
        return instance;
    }

    void SpriteManager::Draw(const Scene& scene, Material* material)
    {
        SpriteManager::GetInstance().m_impl->Draw(scene, material);
    }

}