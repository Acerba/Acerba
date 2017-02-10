#pragma once

#include <Ace/Types.h>

#include <memory>

namespace ace
{
	enum class VertexAttributes
	{
		Position,
		UV,
		Color,
	
		COUNT,
	};

	static const UInt32 vertexAttributeSizes[] = { sizeof(Vector3), sizeof(Vector2), sizeof(Color32) };
	static const char* vertexAttributeNames[] = {"a_position", "a_uv", "a_color"};

	struct Vertex
	{
		Vector3 position;
		Vector2 uv;
		Color32 color;
	};

	enum class BufferType
	{
		Vertex,
		Index,
	};

	enum class BufferUsage
	{
		Static,
		Dynamic,
		Streaming
	};

	// TODO: Dynamic Buffer? -> Stores buffer data inside a vector.

	struct Buffer
	{
		struct BufferImpl;
		std::shared_ptr<BufferImpl> impl;

		BufferType type;
		UInt32 size;

		Buffer() : impl(nullptr), size(0)
		{

		}
	};
}