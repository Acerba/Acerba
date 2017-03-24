#pragma once

#include <Ace/Types.h>

#include <Ace/GraphicsObject.h>

namespace ace
{
	/**
	@brief Vertex Attribute Properties
	*/
	enum class VertexAttributes
	{
		Position,
		UV,
		Color,
	
		COUNT,
	};

	/**
	@brief Vertex Attribute Sizes
	*/
	static const UInt32 vertexAttributeSizes[] = { sizeof(Vector4), sizeof(Vector2), sizeof(Color32) };

	/**
	@brief Vertex Attribute Names
	*/
	static const char* vertexAttributeNames[] = {"a_position", "a_uv", "a_color"};

	/**
	@brief Vertex Structure
	*/
	struct Vertex
	{
		Vector4 position;
		Vector2 uv;
		Color32 color;
	};


	/**
	@brief Buffer Types
	*/
	enum class BufferType
	{
		Vertex,
		Index,
	};

	/**
	@brief Buffer Usage
	*/
	enum class BufferUsage
	{
		Static,
		Dynamic,
		Streaming
	};

	// TODO: Dynamic Buffer? -> Stores buffer data inside a vector.

	/**
	@brief Graphics Buffer
	*/
	struct Buffer : public GraphicsObject<struct BufferImpl>
	{
		BufferType type;
		UInt32 size;

		Buffer(BufferType type = BufferType::Vertex);

		Buffer(BufferImpl* impl);

	protected:
		virtual void Init() const;
	};
}