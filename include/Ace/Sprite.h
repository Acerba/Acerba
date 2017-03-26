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
		
		// TODO: Update all Verticies
		// void Rotate(float deg);
		// void Scale(const Vector2& scale);
		// void Move(const Vector3& pos);
		// void Colorize(const Color& color);
		// void UV(const Rect& uv); // Set UV Rect. (Better method name?) // TODO: Rect (Rectangle)
		// void FlipUV();
		// etc.

		// TODO: Update Vertex by Index.
		// void SetUV(UInt8, const Vector2& uv);
		// void SetColor(UInt8 index, const Color& color);
		// void SetPosition(UInt8 index, const Vector2& pos);
		// etc.

		// TODO: Get methods? (Low priority)

    };

}