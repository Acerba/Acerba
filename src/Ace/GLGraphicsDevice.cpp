#include <Ace/GraphicsDevice.h>
#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/Log.h>

#include <Ace/GL.h>
#include <Ace/GLBufferImpl.h>
#include <Ace/GLShaderImpl.h>
#include <Ace/GLMaterialImpl.h>
#include <Ace/GLTextureImpl.h>

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

		// check OpenGL error
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::string error;

			switch (err) {
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
			}

			Logger::Log(Logger::Priority::Warning, "%s", error.c_str());
		}
	}

	void GraphicsDevice::Viewport(UInt32 w, UInt32 h)
	{
		glViewport(0, 0, w, h);
	}

	Buffer GraphicsDevice::CreateBuffer(BufferType type)
	{
		Buffer buffer;
		buffer.impl.reset(new Buffer::BufferImpl());
		buffer.type = type;
		return buffer;
	}

	void GraphicsDevice::BufferData(Buffer& buffer, UInt32 count, const Vertex* data)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];
		buffer.size = count;

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

	void GraphicsDevice::SetBuffer(const Buffer& buffer, BufferType type)
	{
		if (buffer.impl == nullptr)
		{
			return;
		}

		UInt32 target = GLBufferTargets[static_cast<UInt32>(type)];
		glBindBuffer(target, buffer.impl->bufferID);
	}

	void GraphicsDevice::SetBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, buffer.type);
	}

	void GraphicsDevice::SetVertexBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, BufferType::Vertex);

		//glVertexAttribPointer(0, 3, GL_FLOAT, false, vertexAttributeSizes[0], nullptr);
		//glVertexAttribPointer(1, 2, GL_FLOAT, false, vertexAttributeSizes[0] + vertexAttributeSizes[1], nullptr);
		//glVertexAttribPointer(2, 4, GL_FLOAT, false, vertexAttributeSizes[0] + vertexAttributeSizes[1] + vertexAttributeSizes[2], nullptr);
		//
		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);
	}

	void GraphicsDevice::SetIndexBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, BufferType::Index);
	}

	Texture GraphicsDevice::CreateTexture()
	{
		Texture texture;
		texture.impl.reset(new Texture::TextureImpl());

		return texture;
	}

	void GraphicsDevice::UpdateTexture(Texture& texture, const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format)
	{
		glBindTexture(GL_TEXTURE_2D, texture.impl->textureID);

		// TODO: Format & Filters

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GraphicsDevice::SetTexture(Material& material, const Texture& texture)
	{
		const UInt32 ID = 0;

		glUseProgram(material.impl->materialID);
		glActiveTexture(GL_TEXTURE0 + ID);
		glBindTexture(GL_TEXTURE_2D, texture.impl->textureID);

		UInt32 i = glGetUniformLocation(material.impl->materialID, "Texture");
		glUniform1i(i, ID);

	}

	Shader GraphicsDevice::CreateShader(const char* source, ShaderType type)
	{
		Shader shader;

		shader.impl.reset(new Shader::ShaderImpl(GLShaderTypes[static_cast<UInt32>(type)]));
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
		material.impl.reset(new Material::MaterialImpl());

		if (vertex.impl)
		{
			glAttachShader(material.impl->materialID, vertex.impl->shaderID);
		}

		if (fragment.impl)
		{
			glAttachShader(material.impl->materialID, fragment.impl->shaderID);
		}

		for (int i = 0; i < (int)VertexAttributes::COUNT; ++i)
		{
			glBindAttribLocation(material.impl->materialID, i, vertexAttributeNames[i]);
		}

		glLinkProgram(material.impl->materialID);
	
		if (vertex.impl)
		{
			glDetachShader(material.impl->materialID, vertex.impl->shaderID);
		}
		
		if (fragment.impl)
		{
			glDetachShader(material.impl->materialID, fragment.impl->shaderID);
		}
		
		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;

		glGetProgramiv(material.impl->materialID, GL_LINK_STATUS, &result);
		glGetProgramiv(material.impl->materialID, GL_INFO_LOG_LENGTH, &errorMsgLength);

		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetProgramInfoLog(material.impl->materialID, errorMsgLength, NULL, errorMsg);

			Logger::Log(Logger::Priority::Warning, "%s", errorMsg);
			delete[] errorMsg;

			material.impl = nullptr;

			// TODO: Set default error material.
		}		
		return material;
	}

	void GraphicsDevice::Draw(Material& material, UInt32 elements, UInt32 indicies)
	{
		glUseProgram(material.impl->materialID);

		if (indicies == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, elements);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, indicies, GL_UNSIGNED_INT, 0);
		}
	}

	void GraphicsDevice::Draw(Material& material, const Mesh& mesh)
	{
		SetBuffer(mesh.vertexBuffer);
		SetBuffer(mesh.indexBuffer);

		Draw(material, mesh.GetElements(), mesh.GetIndicies());
	}
}
