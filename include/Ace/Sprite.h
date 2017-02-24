#pragma once

#include <Ace/Buffer.h>

#include <array>

namespace ace
{

    class Sprite
    {
    public:

        std::array<Vertex, 4u> vertexData;

        Sprite(const Vertex(&data)[4]);

        Sprite(const std::array<Vertex, 4u>& data);

    };

}