#pragma once

#include <Ace/Buffer.h>

namespace ace
{
	/**
	@brief Graphics Mesh
	*/
	struct Mesh
	{
		Buffer vertexBuffer;
		Buffer indexBuffer;

		/**
			@brief Default Constructor
			Creates an empty mesh.
		*/
		Mesh()
		{

		}
		
		/**
			@return Vertex Buffer size
		*/
		inline UInt32 GetElements() const
		{
			return vertexBuffer.size;
		}

		/**
			@return Index Buffer size
		*/
		inline UInt32 GetIndicies() const
		{
			return indexBuffer.size;
		}
	};
}