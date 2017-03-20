#pragma once

#include <Ace/Buffer.h>

#include <array>

namespace ace
{

    class Sprite
    {
    public:

        std::array<Vertex, 4u> vertexData;

		Sprite();

		Sprite(float deg);

        /**
        @Brief Sprite constructor.
        @param[in] data Array of 4 vertices by reference.
        */
        Sprite(const Vertex(&data)[4]);

        Sprite(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);

    };

}