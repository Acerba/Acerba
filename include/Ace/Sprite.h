#pragma once

#include <Ace/Buffer.h>

#include <array>

namespace ace
{

    class Sprite
    {
    public:

        std::array<Vertex, 4u> vertexData;

        /**
        @Brief Sprite constructor.
        @param[in] data Array of 4 vertices by reference.
        */
        Sprite(const Vertex(&data)[4]);

        /**
        @Brief Sprite constructor.
        @param[in] data std::array of 4 vertices by reference.
        */
        Sprite(const std::array<Vertex, 4u>& data);

    };

}