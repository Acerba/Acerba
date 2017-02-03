#include <Ace/GraphicsDevice.h>
#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/Log.h>

#include <Ace/GL.h>
#include <Ace/GLBufferImpl.h>
#include <Ace/GLShaderImpl.h>
#include <Ace/GLMaterialImpl.h>

namespace ace
{
	static const UInt32 GLBufferTargets[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
	static const UInt32 GLShaderTypes[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

	void GraphicsDevice::Clear(const Color32& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsDevice::Present(Window& window)
	{
		SDL_GL_SwapWindow((*window)->sdlWindow);
	}

	void GraphicsDevice::Viewport(UInt32 w, UInt32 h)
	{
		glViewport(0, 0, w, h);
	}

	Buffer GraphicsDevice::CreateBuffer(BufferType type)
	{
		Buffer buffer;
		buffer.impl = std::make_shared<Buffer::BufferImpl>(Buffer::BufferImpl());
		buffer.type = type;
		return buffer;
	}

	void GraphicsDevice::BufferData(Buffer& buffer, UInt32 count, const Vertex* data)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];

		glBindBuffer(target, buffer.impl->bufferID);
		glBufferData(target, count * sizeof(Vertex), data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)vertexAttributeSizes[0]);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(vertexAttributeSizes[0] + vertexAttributeSizes[1]));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//glBindBuffer(target, 0);
	}

	void GraphicsDevice::SetVertexBuffer(const Buffer& buffer)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];
		glBindBuffer(target, buffer.impl->bufferID);

		//glVertexAttribPointer(0, 3, GL_FLOAT, false, vertexAttributeSizes[0], nullptr);
		//glVertexAttribPointer(1, 2, GL_FLOAT, false, vertexAttributeSizes[0] + vertexAttributeSizes[1], nullptr);
		//glVertexAttribPointer(2, 4, GL_FLOAT, false, vertexAttributeSizes[0] + vertexAttributeSizes[1] + vertexAttributeSizes[2], nullptr);
		//
		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);
	}

	Shader GraphicsDevice::CreateShader(const char* source, ShaderType type)
	{
		Shader shader;

		shader.impl = std::make_shared<Shader::ShaderImpl>(Shader::ShaderImpl());

		shader.impl->shaderID = glCreateShader(GLShaderTypes[static_cast<UInt32>(type)]);
		shader.type = type;
		
		glShaderSource(shader.impl->shaderID, 1, &source, NULL);
		glCompileShader(shader.impl->shaderID);

		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;

		glGetShaderiv(shader.impl->shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader.impl->shaderID, GL_INFO_LOG_LENGTH, &errorMsgLength);

		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetShaderInfoLog(shader.impl->shaderID, errorMsgLength, NULL, errorMsg);
			Logger::Log(Logger::Priority::Error, "%s", errorMsg);

			delete[] errorMsg;
			shader.impl = nullptr;
		}

		return shader;
	}

	Material GraphicsDevice::CreateMaterial(const Shader& vertex, const Shader& fragment)
	{
		Material material;
		material.impl = std::make_shared<Material::MaterialImpl>(Material::MaterialImpl());

		glAttachShader(material.impl->materialID, vertex.impl->shaderID);
		glAttachShader(material.impl->materialID, fragment.impl->shaderID);

		for (int i = 0; i < (int)VertexAttributes::COUNT; ++i)
		{
			glBindAttribLocation(material.impl->materialID, i, vertexAttributeNames[i]);
		}

		glLinkProgram(material.impl->materialID);
		
		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;

		glGetProgramiv(material.impl->materialID, GL_LINK_STATUS, &result);
		glGetProgramiv(material.impl->materialID, GL_INFO_LOG_LENGTH, &errorMsgLength);

		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetProgramInfoLog(material.impl->materialID, errorMsgLength, NULL, errorMsg);

			Logger::Log(Logger::Priority::Info, "%s", errorMsg);
			delete[] errorMsg;

			material.impl = nullptr;

			// TODO: Set default error material.
		}
		
		return material;
	}

	void GraphicsDevice::Draw(Material& material, UInt32 elements)
	{
		glUseProgram(material.impl->materialID);
		glDrawArrays(GL_TRIANGLES, 0, elements);
	}
}
