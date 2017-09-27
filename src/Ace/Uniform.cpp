#include <Ace/UniformImpl.h>
#include <vector>
#include <string.h>

namespace ace
{
    const UInt32 UniformSizes[] =
    {
        sizeof(Int32),
        sizeof(UInt32),
        sizeof(float),
        sizeof(Vector2),
        sizeof(Vector3),
        sizeof(Vector4),
        sizeof(Matrix2),
        sizeof(Matrix3),
        sizeof(Matrix4),
    };

    struct UniformStorage
    {
        inline static UniformStorage& GetStorage()
        {
            static UniformStorage s_storage;
            return s_storage;
        }

        std::vector<UniformData> uniforms;

        ~UniformStorage()
        {
            for (UInt32 i = 0; i < uniforms.size(); ++i)
            {
                Free(uniforms[i]);
            }
        }

        inline UniformData* Get(const char* name)
        {
            for (UInt32 i = 0; i < uniforms.size(); ++i)
            {
                if (strcmp(uniforms[i].name, name) == 0)
                {
                    return &uniforms[i];
                }
            }

            return nullptr;
        }

        inline void Free(UniformData& uniform)
        {
            free(uniform.data);
            uniform.allocated = uniform.count = 0;
            uniform.data = nullptr;         
        }

        inline void Copy(UniformData& uniform, UniformType type, const void* data, UInt16 count)
        {
            UInt16 size = UniformSizes[static_cast<int>(type)] * count;

            if (uniform.data == nullptr)
            {
                uniform.allocated = size;
                uniform.data = malloc(uniform.allocated);
            }
            else if (size > uniform.allocated)
            {
                Free(uniform);
                uniform.allocated = size;
                uniform.data = malloc(uniform.allocated);           
            }

            uniform.type = type;
            uniform.count = count;

            memcpy(uniform.data, data, size);
        }

        inline UniformData* Add(const char* name)
        {
            UniformData uniform;
            uniform.data = nullptr;
            strncpy(uniform.name, name, sizeof(uniform.name));

            uniforms.push_back(uniform);
            return &uniforms[uniforms.size() - 1];
        }
    };

    void UniformData::AddUniform(const char* name, UniformType type, const void* data, UInt16 count)
    {
        UniformStorage& storage = UniformStorage::GetStorage();

        UniformData* uniform = storage.Get(name);

        if (uniform == nullptr)
        {
            uniform = storage.Add(name);
        }

        storage.Copy(*uniform, type, data, count);
    }

    const UniformData* UniformData::GetUniforms(UInt32& size)
    {
        UniformStorage& storage = UniformStorage::GetStorage();

        size = storage.uniforms.size();
        return storage.uniforms.data();
    }
}