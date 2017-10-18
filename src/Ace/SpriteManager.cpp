#include <Ace/SpriteManager.h>

#include <Ace/Camera.h>
#include <Ace/Component.h>
#include <Ace/ComponentPool.h>
#include <Ace/Entity.h>
#include <Ace/EntityHandle.h>
#include <Ace/EntityManager.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/Math.h>
#include <Ace/Transform.h>

#include <algorithm>
#include <iterator>


namespace ace
{
	std::vector<math::Matrix4> matrix;


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

	const Material& GetTargetMaterial(const Material& material, const Camera& camera, const Matrix4& model)
	{
		material.Uniform("M", model);
		return material;
	}


	const Material& GetTargetMaterial(const Material& material, const Camera& camera, UInt32 offset = 0u, UInt32 count = 64u)
	{
	   // material.Uniform("VP", camera.GetMatrix());
		material.Uniform("M", matrix.data() + offset, count);
		return material;
	}


	void SpriteManager::DrawDrawables(const Scene& scene, const Camera& camera, const Material* customMaterial)
	{
		EntityManager::ComponentPool<Material>& primaryPool = EntityManager::ComponentPool<Material>::GetPool();
		EntityManager::ComponentPool<Drawable*>& secondaryPool = EntityManager::ComponentPool<Drawable*>::GetPool();
		EntityManager::ComponentBaseHandle* secondary = nullptr;
		Drawable* drawable = nullptr;

		//Find all entities that have both material and sprite
		for (UInt32 i = 0u; i < primaryPool.m_components.size(); ++i)
		{
			if (primaryPool.m_handles[i]->entity->ComponentCount() > 1u)
			{
				if ((secondary = primaryPool.m_handles[i]->entity->GetComponentHandle<Drawable*>()) == nullptr)
				{
					continue;
				}

				drawable = secondaryPool.m_components[secondary->index];

				if (drawable != nullptr)
				{
					GraphicsDevice::SetMaterial(GetTargetMaterial(customMaterial ? *customMaterial : primaryPool.m_components[i], camera, secondary->entity->transform.model));
					drawable->Draw();
				}
			}
		}
	}

    void SpriteManager::DrawImpl(const Scene& scene, const Camera& camera, const Material* customMaterial)
    {
        static const UInt32 maxCount = 64u;
        std::vector<Group> groups(Sort(scene));

        UInt32 count = 0u;
        for (const auto& itr : groups)
            count += (itr.end - itr.start);

        //Checks and grows m_indexTable if needed
        HandleIndices(count);

        //TODO: Change loop and both Draw-functions params to const if GraphicsDevice::Draw material accepts const
        
        for (const auto& itr : groups)
        {
            const UInt32 indexCount = itr.end - itr.start;
            GraphicsDevice::BufferData(m_buffer, indexCount * 4, m_sprites[itr.start].vertexData.data(), BufferUsage::Streaming);
            GraphicsDevice::SetBuffer(m_buffer);

            // for (int i = 0; i < indexCount; ++i)
            // {
            //     auto& mat = GetTargetMaterial(customMaterial ? *customMaterial : itr.material, camera);
            //     matrix[0] = matrix[i];
            //     m_sprites[i].vertexData[0].position.w = 0;
            //     m_sprites[i].vertexData[1].position.w = 0;
            //     m_sprites[i].vertexData[2].position.w = 0;
            //     m_sprites[i].vertexData[3].position.w = 0;
            //     mat.Uniform("M", matrix);
            //     GraphicsDevice::Draw(mat, m_sprites[i]);
            // }
            
            const float timesTemp = static_cast<float>(indexCount) / 64.f;
            const UInt32 times = static_cast<UInt32>(timesTemp) + (math::Abs(timesTemp - math::Floor(timesTemp)) < 0.001f ? 0u : 1u);

            for (UInt32 i = 0; i < times; ++i)
            {
                const UInt32 elementsCount = 64u < (indexCount - (i * 64u)) ? 64u : (indexCount - (i * 64u));
                GraphicsDevice::SetMaterial(GetTargetMaterial(customMaterial ? *customMaterial : itr.material, camera, 64u * i, elementsCount));
                GraphicsDevice::Draw( 0u, elementsCount * 6u, m_indexTable + (elementsCount * 6u * i)); //  + (i * maxCount)
            }

           // const UInt32 index6 = indexCount * 6u;
            //for (UInt32 i = 0u; i < (index6 / maxCount) + 1u; ++i)
            //{
            //    const UInt32 left = index6 - (i * maxCount);
            //    const UInt32 currentcount = left < maxCount ? left : maxCount;
            //
            //    
            //
            //    GraphicsDevice::Draw(GetTargetMaterial(customMaterial ? *customMaterial : itr.material, camera, 64, currentcount), 0u, i*64, m_indexTable + currentcount); //  + (i * maxCount)
            //}
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
                m_indexTable[0u + 6u * j] = 0u + 4u * j;
                m_indexTable[1u + 6u * j] = 1u + 4u * j;
                m_indexTable[2u + 6u * j] = 2u + 4u * j;
                m_indexTable[3u + 6u * j] = 2u + 4u * j;
                m_indexTable[4u + 6u * j] = 3u + 4u * j;
                m_indexTable[5u + 6u * j] = 0u + 4u * j;
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
                m_indexTable[0u + 6u * j] = 0u + 4u * j;
                m_indexTable[1u + 6u * j] = 1u + 4u * j;
                m_indexTable[2u + 6u * j] = 2u + 4u * j;
                m_indexTable[3u + 6u * j] = 2u + 4u * j;
                m_indexTable[4u + 6u * j] = 3u + 4u * j;
                m_indexTable[5u + 6u * j] = 0u + 4u * j;
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


        matrix.clear();

        UInt32 instanceID = 0;

        //Find all entities that have both material and sprite
        for (UInt32 i = 0u; i < primaryPool.m_components.size(); ++i)
        {
            if (primaryPool.m_handles[i]->entity->ComponentCount() > 1u &&
                (secondary = primaryPool.m_handles[i]->entity->GetComponentHandle<Sprite>()) != nullptr) 
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
                    instanceID = 0;
                }

                EntityManager::EntityHandle* e = secondaryPool.m_handles[secondary->index]->entity;

                //Temporarily store sprite and its handle
                if (e->manager == (*root)->manager)
                {
                    sprites.emplace_back(secondaryPool.m_components[secondary->index]);
                    sprites.back().SetInstanceID(instanceID++);

                    if (instanceID >= 64)
                    {
                        instanceID = 0;
                    }

                    matrix.emplace_back(e->transform.model);
                    handles.emplace_back(e);
                }
            }
        }

        m_sprites.reserve(sprites.size());

        //Sort spriteindices by Z-value and create sorted m_sprites
        for (const auto& itr : SortIndices(handles))
        {
            m_sprites.emplace_back(sprites[itr]);
        }

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


    void SpriteManager::Draw(const Scene& scene, const Camera& camera, const Material* material)
    {
		GetInstance().DrawDrawables(scene, camera, material);
        GetInstance().DrawImpl(scene, camera, material);

        Material::Uniform("M", Matrix4::Identity()); // Rests Model matrix.
    }

}