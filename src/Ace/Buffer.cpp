#include <Ace/Buffer.h>

namespace ace
{
	Buffer::Buffer(BufferImpl* impl, BufferType type) : Graphics(impl), size(0), type(type)
	{

	}

    void Buffer::operator=(const Buffer& buffer)
    {
        size = buffer.size;
        m_impl = buffer.m_impl;
    }

	VertexBuffer::VertexBuffer(BufferImpl* impl) : Buffer(impl, BufferType::Vertex)
	{

	}

	IndexBuffer::IndexBuffer(BufferImpl* impl) : Buffer(impl, BufferType::Index)
	{

	}
}