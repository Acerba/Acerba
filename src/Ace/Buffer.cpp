#include <Ace/Buffer.h>
#include <Ace/GLBufferImpl.h>

namespace ace
{
	Buffer::Buffer(BufferType type) : Graphics(nullptr), type(type), size(0)
	{

	}

	Buffer::Buffer(BufferImpl* impl) : Graphics(impl), size(0)
	{

	}
}