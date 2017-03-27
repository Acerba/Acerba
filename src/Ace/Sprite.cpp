#include <Ace/Sprite.h>
#include <Ace/Math.h>

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

	Sprite::Sprite(float deg) : Sprite()
	{
		for (int i = 0; i < 4; ++i)
		{
			vertexData[i].position = math::Matrix4::RotationZ(deg) * vertexData[i].position;
		}
	}

    Sprite::Sprite(const Vertex(&data)[4]) :
        vertexData({ data[0], data[1], data[2], data[3] })
    {

    }

    Sprite::Sprite(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) :
        vertexData({ v1, v2, v3, v4 })
    {

    }

	void Sprite::Rotate(float deg)
	{
		for (int i = 0; i < 4; ++i)
		{
			vertexData[i].position = math::Matrix4::RotationZ(deg) * vertexData[i].position;
		}
	}

	void Sprite::Scale(const Vector2& scale)
	{
		for (int i = 0; i < 4; ++i)
		{
			vertexData[i].position.x *= scale.x;
			vertexData[i].position.y *= scale.y;
		}
	}

	void Sprite::Move(const Vector3& pos)
	{
		for (int i = 0; i < 4; ++i)
		{
			vertexData[i].position.x += pos.x;
			vertexData[i].position.y += pos.y;
		}
	}

	void Sprite::Colorize(const Color& color)
	{
		for (int i = 0; i < 4; ++i)
		{
			vertexData[i].color = color;
		}
	}

	void Sprite::UVRect(const Rect& uv)
	{
			vertexData[0].uv.x = uv.x;
			vertexData[0].uv.y = uv.y;
			
			vertexData[1].uv.x = uv.x + uv.height;
			vertexData[1].uv.y = uv.y + uv.height;

			vertexData[2].uv.x = uv.x + uv.height + uv.width;
			vertexData[2].uv.y = uv.y + uv.height + uv.width;

			vertexData[3].uv.x = uv.x + uv.width;
			vertexData[3].uv.y = uv.y + uv.width;
	}

	void Sprite::FlipUV(const bool axis)
	{
		Sprite temp = *this;

		if (axis)
		{
		vertexData[0].uv.x = temp.vertexData[2].uv.x;
		vertexData[1].uv.x = temp.vertexData[3].uv.x;
		vertexData[2].uv.x = temp.vertexData[0].uv.x;
		vertexData[3].uv.x = temp.vertexData[1].uv.x;
		}
		else
		{
		vertexData[0].uv.y = temp.vertexData[2].uv.y;
		vertexData[1].uv.y = temp.vertexData[3].uv.y;
		vertexData[2].uv.y = temp.vertexData[0].uv.y;
		vertexData[3].uv.y = temp.vertexData[1].uv.y;
		}

	}
}