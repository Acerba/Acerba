#pragma once

#include <Ace/Macros.h>

#include <Ace/Color.h>

#include <Ace/Buffer.h>
#include <Ace/Mesh.h>
#include <Ace/Shader.h>
#include <Ace/Material.h>
#include <Ace/Texture.h>
#include <Ace/Framebuffer.h>

namespace ace
{
	class Window;
	
	enum class Features
	{
		Blend		= 0x1,
		Depth		= 0x2,
		Stencil		= 0x4,
		Scissor		= 0x8,

		Default = Blend | Depth,
	};

	enum class ClearFlags
	{
		Color		= 0x1,
		Depth		= 0x2,
		Stencil		= 0x4,

		ALL = Color| Depth | Stencil, 
	};

	ACE_ENUM_OPERATORS(Features)
	ACE_ENUM_OPERATORS(ClearFlags)

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

		static void Enable(bool status, Features);

		static void Clear(const Color32& color, ClearFlags clear = ClearFlags::ALL);
		
		static void Present(Window& window);
		static void Viewport(UInt32 w, UInt32 h);
	
		static void Scissor(Int32 x, Int32 y, UInt32 width, UInt32 height);

		static void Draw(Material& material, UInt32 elements, UInt32 indicies);
		static void Draw(Material& material, const Mesh& mesh);

		static Buffer CreateBuffer(BufferType type);
		static void BufferData(Buffer& buffer, UInt32 count, const Vertex* data, BufferUsage usage = BufferUsage::Static, UInt32 instances = 1);
		static void BufferSubData(Buffer& buffer, UInt32 count, UInt32 offset, const Vertex* data);

		static void SetBuffer(const Buffer&, BufferType type);
		static void SetBuffer(const Buffer&);
		static void SetVertexBuffer(const Buffer&);
		static void SetIndexBuffer(const Buffer&);

		static Shader CreateShader(const char* source, ShaderType type);
		static Material CreateMaterial(const Shader& vertex, const Shader& fragment);
		
		static Texture CreateTexture();
		static void UpdateTexture(Texture&, const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format);
		static void SetTexture(Material&, const Texture&);

		static void Uniform(Material& material, const char* name, const void* data, UniformType uniform, UInt32 elements = 1);

		static Framebuffer CreateFramebuffer();

	private:

		static void SetMaterialFlags(Material& flags);
		static void SetTextureFlags(Texture& flags);
	};
}