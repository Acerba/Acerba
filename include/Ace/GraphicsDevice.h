#pragma once

#include <Ace/Color.h>

#include <Ace/Buffer.h>

namespace ace
{
	class Window;
	
	class GraphicsDevice
	{
	public:

		/*
			TODO:

			Buffers
			- Create
			- BufferData
			- SubBufferData
			- Set

			Textures
			- Create
			- Update
			- Set

			Framebuffers
			- Create
			- Set
		*/

		static void Clear(const Color32& color);
		static void Present(Window& window);
		static void Draw(UInt32 elements);
		static void Viewport(UInt32 w, UInt32 h);

		static Buffer CreateBuffer(BufferType type);
		static void BufferData(Buffer& buffer, UInt32 count, Vertex* data);

		static void SetVertexBuffer(const Buffer&);
	};
}