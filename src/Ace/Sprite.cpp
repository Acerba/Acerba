#include <Ace/Sprite.h>

namespace ace
{

    const ace::Vertex triangle[4] = {
        { { 0.5f, 0.5f, 0, 0.0f }, { 1.0f, 0.0f }, 0xFFFFFFFFU },
        { { -0.5f, 0.5f, 0, 0.0f }, { 0.0f, 0.0f }, 0xFFFFFFFFU },
        { { -0.5f, -0.5f, 0, 0.0f }, { 0.0f, 1.0f }, 0xFFFFFFFFU },
        { { 0.5f, -0.5f, 0, 0.0f }, { 1.0f, 1.0f }, 0xFFFFFFFFU },
    };

    Sprite::Sprite() : Sprite(triangle)
    {

    }

    Sprite::Sprite(const Vertex(&data)[4]) :
        vertexData({ data[0], data[1], data[2], data[3] })
    {

    }

    Sprite::Sprite(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) :
        vertexData({ v1, v2, v3, v4 })
    {

    }



}