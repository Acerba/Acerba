#pragma once

#include <Ace/Macros.h>

#include <Ace/Color.h>

#include <Ace/Buffer.h>
#include <Ace/Mesh.h>
#include <Ace/Shader.h>
#include <Ace/Material.h>
#include <Ace/Texture.h>
#include <Ace/Framebuffer.h>

#include <Ace/Sprite.h>

namespace ace
{
	class Window;
	
	/**
		@brief Feature Flags
	*/
	enum class Features
	{
		Blend		= 0x1,
		Depth		= 0x2,
		Stencil		= 0x4,
		Scissor		= 0x8,

		Default = Blend | Depth,
	};

	/**
		@brief Clear Flags
	*/
	enum class ClearFlags
	{
		Color		= 0x1,
		Depth		= 0x2,
		Stencil		= 0x4,

		ALL = Color| Depth | Stencil, 
	};

	ACE_ENUM_OPERATORS(Features)
	ACE_ENUM_OPERATORS(ClearFlags)

	/**
		@brief Graphics Device
	*/
	class GraphicsDevice
	{
	public:

		/*
			TODO:
			Framebuffers
			- Set
		*/


		/**
			@brief Enable/Disable Feature
			@param[in] status True enables feature and False disables feature
			@param[in] features Flags
		*/
		static void Enable(bool status, Features);

		/**
			@brief Clears Graphics Context
			@param[in] color Clear Color
			@param[in] clear Clear Flags
		*/
		static void Clear(const Color32& color, ClearFlags clear = ClearFlags::ALL);
		
		/**
			@brief Present Graphics Context
			@param[in] window
		*/
		static void Present(Window& window);

		/**
			@brief Sets Viewport
			@param[in] width
			@param[in] height
		*/
		static void Viewport(UInt32 width, UInt32 height);
	
		/**
			@brief Sets Scissor
			@param[in] x
			@param[in] y
			@param[in] width
			@param[in] height
		*/
		static void Scissor(Int32 x, Int32 y, UInt32 width, UInt32 height);

		/**
		@brief Draw using Sprites
		@see SetVertexBuffer
		@param[in] material
		@param[in] sprite
		*/
		static void Draw(Material& material, const Sprite& sprite);

		/**
			@brief Draw using Buffers
			@see SetBuffer
			@see SetVertexBuffer
			@param[in] material
			@param[in] elements
			@param[in] indicies
			@param[in] indexTable
		*/
		static void Draw(Material& material, UInt32 elements, UInt32 indicies, const UInt32* indexTable = nullptr);

		/**
			@brief Draw using Mesh
			@param[in] material
			@param[in] mesh
		*/
		static void Draw(Material& material, const Mesh& mesh);


		/**
			@brief CreateBuffer
			@param[in] type BufferType
			@return Buffer
		*/
		static Buffer CreateBuffer(BufferType type);

		/**
			@brief Buffer Data
			@param[in, out] buffer
			@param[in] count Vertex Count
			@param[in] data Vertex Data
			@param[in] usage Buffer Usage
			@param[in] instances
		*/
		static void BufferData(Buffer& buffer, UInt32 count, const Vertex* data, BufferUsage usage = BufferUsage::Static, UInt32 instances = 0);

		/**
			@brief BufferSubData
			@param[in,out] buffer
			@param[in] count Vertex Count
			@param[in] offset Vertex Offset
			@param[in] data Vertex Data
		*/
		static void BufferSubData(Buffer& buffer, UInt32 count, UInt32 offset, const Vertex* data);

		/**
			@brief Set Buffer
			@param[in] buffer
			@param[in] type
		*/
		static void SetBuffer(const Buffer&, BufferType type);

		/**
			@brief Set Buffer
			@param[in] buffer
		*/
		static void SetBuffer(const Buffer&);

		/**
			@brief Set Vertex Buffer
			@param[in] vertexBuffer
		*/
		static void SetVertexBuffer(const Buffer&);

		/**
			@brief Set Index Buffer
			@param[in] indexBuffer
		*/
		static void SetIndexBuffer(const Buffer&);

		/**
			@brief Create Shader
			@param[in] source Shader Source
			@param[in] type Shader Type
			@return Shader
		*/
		static Shader CreateShader(const char* source, ShaderType type);

		/**
			@brief Create Material
			@param[in] vertex
			@param[in] fragment
			@return Material
		*/
		static Material CreateMaterial(const Shader& vertex, const Shader& fragment);
		
		/**
			@return Material
		*/
		static Material CreateMaterial();


		/**
			@brief Create Texture
			@return Texture
		*/
		static Texture CreateTexture();

		/**
			@brief Update Texture
			@param[in,out] texture
			@param[in] pixels Pixels
			@param[in] width
			@param[in] height
			@param[in] format Pixel Format
		*/
		static void UpdateTexture(Texture&, const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format);

		/**
			@brief Set Texture
			@param[in,out] material
			@param[in] texture
		*/
		static void SetTexture(Material&, const Texture&, const char* name, UInt8 id);

		/**
			@brief Uniform
			@param[in,out] material
			@param[in] name
			@param[in] data
			@param[in] uniform Uniform Type
			@param[in] elements Count
		*/
		static void Uniform(const Material& material, const char* name, const void* data, UniformType uniform, UInt32 elements = 1);

		/**
			@brief Create Framebuffer
			@return Framebuffer
		*/
		static Framebuffer CreateFramebuffer();
		static void SetFramebuffer(Framebuffer* framebuffer);
		static void SetFramebufferTarget(Framebuffer& framebuffer, Texture&, UInt32 framebufferAttachment);

	private:

		static void SetMaterialFlags(Material& flags);
		static void SetTextureFlags(Texture& flags);
	};
}