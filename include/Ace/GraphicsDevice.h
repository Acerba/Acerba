#pragma once

#include <Ace/Color.h>

#include <Ace/Buffer.h>
#include <Ace/Mesh.h>
#include <Ace/Shader.h>
#include <Ace/Material.h>
#include <Ace/Texture.h>

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
		static void Viewport(UInt32 w, UInt32 h);

		static void Draw(Material& material, UInt32 elements, UInt32 indicies);
		static void Draw(Material& material, const Mesh& mesh);

		static Buffer CreateBuffer(BufferType type);
		static void BufferData(Buffer& buffer, UInt32 count, const Vertex* data);

		static void SetBuffer(const Buffer&, BufferType type);
		static void SetBuffer(const Buffer&);
		static void SetVertexBuffer(const Buffer&);
		static void SetIndexBuffer(const Buffer&);

		static Shader CreateShader(const char* source, ShaderType type);
		static Material CreateMaterial(const Shader& vertex, const Shader& fragment);

		static Texture CreateTexture();
		static void UpdateTexture(Texture&, const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format);
		static void SetTexture(Material&, const Texture&);
	};
}