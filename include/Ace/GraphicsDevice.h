#pragma once

#include <Ace/Color.h>

#include <Ace/Buffer.h>
#include <Ace/Shader.h>
#include <Ace/Material.h>

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
		static void Draw(Material& material, UInt32 elements);
		static void Viewport(UInt32 w, UInt32 h);

		static Buffer CreateBuffer(BufferType type);
		static void BufferData(Buffer& buffer, UInt32 count, const Vertex* data);

		static void SetVertexBuffer(const Buffer&);

		static Shader CreateShader(const char* source, ShaderType type);
		static Material CreateMaterial(const Shader& vertex, const Shader& fragment);

	};
}