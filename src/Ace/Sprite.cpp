#include <Ace/Sprite.h>

namespace ace
{

    Sprite::Sprite(const Vertex(&data)[4]) :
        vertexData({ data[0], data[1], data[2], data[3] })
    {

    }

    Sprite::Sprite(const std::array<Vertex, 4u>& data) :
        vertexData(data)
    {

    }

}