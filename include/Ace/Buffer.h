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
	static const UInt32 vertexAttributeSizes[] = 
	{ 
		sizeof(Vector4), 
		sizeof(Vector2), 
		sizeof(Color32) 
	};

	/**
	@brief Vertex Attribute Names
	*/
	static const char* vertexAttributeNames[] = 
	{
		"a_position", 
		"a_uv", 
		"a_color"
	};

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

	/**
	@brief Generic Graphics Buffer
	*/
	struct Buffer : public GraphicsObject<struct BufferImpl>
	{
		const BufferType type;
		UInt32 size;

		Buffer(BufferImpl* impl, BufferType type);

	protected:
		virtual void Init() const;
	};

	/**
	@brief Vertex Buffer
	*/
	struct VertexBuffer : public Buffer
	{
		VertexBuffer(BufferImpl* impl = nullptr);
	};

	/**
	@brief Index Buffer
	*/
	struct IndexBuffer : public Buffer
	{
		IndexBuffer(BufferImpl* impl = nullptr);
	};
}