#include <Ace/SpriteManager.h>

#include <Ace/Component.h>
#include <Ace/ComponentPool.h>
#include <Ace/Entity.h>
#include <Ace/EntityHandle.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/Transform.h>

#include <algorithm>
#include <iterator>


namespace ace
{


    SpriteManager::Group::Group(const Material& mat, const UInt32 begin) :
        material(mat),
        start(begin),
        end(begin + 1u)
    {

    }


    SpriteManager::SpriteManager() :
        m_sprites(),
        m_buffer(GraphicsDevice::CreateBuffer(BufferType::Vertex)),
        m_indexTable(nullptr),
        m_size(0u)
    {

    }


    SpriteManager::~SpriteManager()
    {
        if (m_indexTable)
        {
            delete[]m_indexTable;
            m_indexTable = nullptr;
        }
    }


    void SpriteManager::DrawImpl(const Scene& scene, Material* material)
    {
        std::vector<Group> groups(Sort(scene));

        //Checks and grows m_indexTable if needed
        HandleIndices(std::distance(groups.begin(), std::max_element(groups.begin(), groups.end(),
            [](const Group& g1, const Group& g2){return g1.end - g1.start < g2.end - g2.start; })));

        //TODO: Change loop and both Draw-functions params to const if GraphicsDevice::Draw material accepts const
        for (auto& itr : groups)
        {
            const UInt32 indexCount = itr.end - itr.start;
            GraphicsDevice::BufferData(m_buffer, indexCount * 4, m_sprites[itr.start].vertexData.data(), BufferUsage::Streaming);
            GraphicsDevice::SetBuffer(m_buffer);
            GraphicsDevice::Draw(material ? *material : itr.material, 0u, indexCount * 6u, m_indexTable);
        }

        //TODO: Add logic to check if the sprites may be saved
        m_sprites.clear();
    }


    SpriteManager& SpriteManager::GetInstance()
    {
        static SpriteManager instance;
        return instance;
    }


    void SpriteManager::HandleIndices(UInt32 newSize)
    {
        if (newSize == 0u)
            newSize = 1u;

        if (!m_indexTable)
        {
            m_size = newSize;
            m_indexTable = new UInt32[newSize * 6u];
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
            UInt32* temp = new UInt32[m_size * 6u];
            std::copy(m_indexTable, m_indexTable + (m_size - 1u), temp);
            delete[]m_indexTable;
            m_indexTable = new UInt32[newSize * 6u];
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


    std::vector<SpriteManager::Group> SpriteManager::Sort(const Scene& scene)
    {
        //Temp storage
        std::vector<EntityManager::EntityHandle*> handles;
        std::vector<Sprite> sprites;
        std::vector<Group> groups;
        EntityManager::ComponentPool<Material>& primaryPool = EntityManager::ComponentPool<Material>::GetPool();
        EntityManager::ComponentPool<Sprite>& secondaryPool = EntityManager::ComponentPool<Sprite>::GetPool();
        EntityManager::ComponentBaseHandle* secondary = nullptr;
        const Entity* root = &scene.GetRoot();


        //Find all entities that have both material and sprite
        for (UInt32 i = 0u; i < primaryPool.m_components.size(); ++i)
        {
            if (primaryPool.m_handles[i]->entity->Count() > 1u && (secondary = primaryPool.m_handles[i]->entity->Get<Sprite>()) != nullptr)
            {
                bool added = false;
                UInt32 start = 0u;

                // FIXME:
                //Group size grows
                for (auto& itr : groups)
                {
                    start = itr.end;
                    if (*itr.material == *primaryPool.m_components[i])
                    {
                        start = ++itr.end;
                        added = true;
                        break;
                    }
                }
                //New material, new group
                if (added == false)
                {
                    groups.emplace_back(primaryPool.m_components[i], start);
                }

                EntityManager::EntityHandle* e = secondaryPool.m_handles[secondary->index]->entity;

                //Temporarily store sprite and its handle
                if (e->manager == (*root)->manager)
                {
                    sprites.emplace_back(secondaryPool.m_components[secondary->index]);
                    handles.emplace_back(e);
                }
            }
        }

        // FIXME:
        //groups[0].start = 0;
        //groups[0].end = 2;

        m_sprites.reserve(sprites.size());

        //Sort spriteindices by Z-value and create sorted m_sprites
        for (const auto& itr : SortIndices(handles))
        {
            m_sprites.emplace_back(sprites[itr]);
        }
        //m_sprites.insert(m_sprites.begin(), sprites.begin(), sprites.end());

        return groups;
    }


    std::vector<UInt32> SpriteManager::SortIndices(const std::vector<EntityManager::EntityHandle*>& handles) const
    {
        std::vector<UInt32> indices(handles.size());
        for (UInt32 i = 0u; i < indices.size(); ++i)
            indices[i] = i;

        std::sort(indices.begin(), indices.end(),
            [&handles](const UInt32 i1, const UInt32 i2)
        {
            if (i1 == i2)
                return false;
            else if (handles[i1]->transform.position.z > handles[i2]->transform.position.z)
                return true;
            else
                return false;
        }
        );

        return indices;
    }


    void SpriteManager::Draw(const Scene& scene, Material* material)
    {
        GetInstance().DrawImpl(scene, material);
    }

}