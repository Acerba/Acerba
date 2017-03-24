#include <Ace/Shader.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/GLShaderImpl.h>

namespace ace
{
	Shader::Shader(ShaderImpl* impl) : Graphics(impl)
	{

	}

	Shader::Shader(const File& file, ShaderType type) : Graphics(nullptr)
	{
		Load(file, type);
	}

	bool Shader::Load(const File& file, ShaderType type)
	{
		auto buffer = file.ReadAllText();
		return Create(buffer.get(), type);
	}

	bool Shader::Create(const char* source, ShaderType type)
	{
		*this = GraphicsDevice::CreateShader(source, type);
		bool status = (*this);
		return status != false;
	}
}