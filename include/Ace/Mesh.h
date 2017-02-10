#pragma once

#include <Ace/Buffer.h>

namespace ace
{
	struct Mesh
	{
		Buffer vertexBuffer;
		Buffer indexBuffer;

		Mesh()
		{

		}
		
		inline UInt32 GetElements() const
		{
			return vertexBuffer.size;
		}

		inline UInt32 GetIndicies() const
		{
			return indexBuffer.size;
		}
	};
}