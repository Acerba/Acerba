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

	// TODO: Better names.
	static const char* vertexAttributeNames[] = {"position", "uv", "color"};

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

	struct Buffer
	{
		struct BufferImpl;
		std::shared_ptr<BufferImpl> impl;

		BufferType type;
		UInt32 size;
	};
}