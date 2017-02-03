#pragma once

#include <Ace/Shader.h>
#include <Ace/GL.h>


namespace ace
{
	struct Shader::ShaderImpl
	{
		UInt32 shaderID;

		ShaderImpl(UInt32 type) : shaderID(glCreateShader(type))
		{
			
		}

		~ShaderImpl()
		{
			glDeleteShader(shaderID);
		}
	};
}