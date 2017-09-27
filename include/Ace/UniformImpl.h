#pragma once

#include <Ace/Uniform.h>

namespace ace
{
    struct UniformData
    {
        static void AddUniform(const char* name, UniformType type, const void* data, UInt16 count);
        static const UniformData* GetUniforms(UInt32& size);

        char name[24];
        UniformType type;
        void* data;

        UInt16 count;
        UInt16 allocated;
    };
}