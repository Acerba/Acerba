#pragma once

#include <Ace/Buffer.h>
#include <Ace/GL.h>

namespace ace
{
	struct Buffer::BufferImpl
	{
		UInt32 bufferID;

		BufferImpl()
		{
			glGenBuffers(1, &bufferID);
		}

		~BufferImpl()
		{
			glDeleteBuffers(1, &bufferID);
		}
	};
}