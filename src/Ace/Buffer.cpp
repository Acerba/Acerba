#include <Ace/Buffer.h>

namespace ace
{
	Buffer::Buffer(BufferImpl* impl, BufferType type) : Graphics(impl), size(0), type(type)
	{

	}

	VertexBuffer::VertexBuffer(BufferImpl* impl) : Buffer(impl, BufferType::Vertex)
	{

	}

	IndexBuffer::IndexBuffer(BufferImpl* impl) : Buffer(impl, BufferType::Vertex)
	{

	}
}