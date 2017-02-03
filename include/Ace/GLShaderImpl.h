#pragma once

#include <Ace/Shader.h>
#include <Ace/GL.h>


namespace ace
{
	struct Shader::ShaderImpl
	{
		UInt32 shaderID;

		ShaderImpl()
		{
			
		}

		~ShaderImpl()
		{
			glDeleteShader(shaderID);
		}
	};
}