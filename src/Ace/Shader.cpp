#include <Ace/Shader.h>
#include <Ace/GraphicsDevice.h>

namespace ace
{
	Shader::Shader() : impl(nullptr)
	{

	}

	Shader::Shader(const File& file, ShaderType type)
	{
		Load(file, type);
	}

	bool Shader::Load(const File& file, ShaderType type)
	{
		auto buffer = file.ReadAllText();
		return Init(buffer.get(), type);
	}

	bool Shader::Init(const char* source, ShaderType type)
	{
		*this = GraphicsDevice::CreateShader(source, type);
		return impl != nullptr;
	}
}