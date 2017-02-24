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



    class SpriteManagerImpl
    {

        std::vector<Sprite> m_sprites;

        std::vector<size_t> _sortIndices(const std::vector<EntityManager::EntityHandle*>& handles) const
        {
            std::vector<size_t> indices(m_sprites.size());
            detail::ACE_iota(indices.begin(), indices.end(), 0);

            std::sort(indices.begin(), indices.end(),
                [handles](size_t i1, size_t i2){return handles[i1]->m_transform->position.z > handles[i2]->m_transform->position.z; }
            );

            return indices;
        }

        std::vector<Group> _sort()
        {
            //Temp storage
            std::vector<EntityManager::EntityHandle*> handles = std::vector<EntityManager::EntityHandle*>();
            std::vector<Sprite> sprites = std::vector<Sprite>();
            std::vector<Group> groups = std::vector<Group>();
            EntityManager::ComponentPool<Material>& primaryPool = EntityManager::ComponentPool<Material>::GetPool();
            EntityManager::ComponentPool<Sprite>& secondaryPool = EntityManager::ComponentPool<Sprite>::GetPool();
            EntityManager::ComponentBaseHandle* secondary = nullptr;

            //Find all entities that have both material and sprite
            for (size_t i = 0u; i < primaryPool.m_components.size(); ++i)
            {
                if (primaryPool.m_handles[i]->entity->Count() > 1u && (secondary = primaryPool.m_handles[i]->entity->Get<Sprite>()) != nullptr)
                {
                    bool added = false;

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


            m_sprites.reserve(sprites.size());

            //Sort spriteindices by Z-value and create sorted m_sprites
            for (const auto& itr : _sortIndices(handles))
            {
                m_sprites.emplace_back(sprites[itr]);
            }

            return groups;
        }

        UInt32 _indices(const UInt32 baseIndex)
        {
            UInt32 indices = 0u;

            //TODO: what to iterate over
            for (size_t i = 0u; i < 666u; ++i)
            {
                indices += baseIndex + 6u * i;
            }

            return indices;
        }


    public:

        void Draw(const Scene& scene)
        {

            std::vector<Group> groups(_sort());
            for (size_t i = 0u; i < groups.size(); ++i)
            {
                
                //TODO: buffer?
                //groups[i].material.impl
                Buffer b;

                GraphicsDevice::BufferData(b, groups[i].end - groups[i].start, m_sprites[i].vertexData.data(), BufferUsage::Streaming);
                

                //TODO: change material on the fly


                //TODO: elements count?
                GraphicsDevice::Draw(groups[i].material, 666u, _indices(i));
            }

            //TODO: to erase or not to erase
            //m_sprites.erase(m_sprites.begin(), m_sprites.end());
        }

        SpriteManagerImpl() :
            m_sprites()
        {

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

    void SpriteManager::Draw(const Scene& scene)
    {
        SpriteManager::GetInstance().m_impl->Draw(scene);
    }

}