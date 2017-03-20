#include <Ace/GraphicsObject.h>
#include <Ace/GraphicsDevice.h>

#include <Ace/Buffer.h>

namespace ace
{
	void Buffer::Init() const
	{
		InitImpl(GraphicsDevice::CreateBuffer(type));
	}

	void Texture::Init() const
	{
		InitImpl(GraphicsDevice::CreateTexture());
	}

	void Shader::Init() const
	{
		// Shader need source.
		// So no "Default" init.
	}

	void Material::Init() const
	{
		InitImpl(GraphicsDevice::CreateMaterial());
	}

	void Framebuffer::Init() const
	{
		InitImpl(GraphicsDevice::CreateFramebuffer());
	}
}