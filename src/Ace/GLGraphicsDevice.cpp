#include <Ace/GraphicsDevice.h>
#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/Log.h>

#include <Ace/GL.h>
#include <Ace/GLBufferImpl.h>
#include <Ace/GLShaderImpl.h>
#include <Ace/GLMaterialImpl.h>
#include <Ace/GLTextureImpl.h>
#include <Ace/GLFramebbuferImpl.h>

#include <Ace/Platform.h>
#include <Ace/Assert.h>


namespace ace
{
	template <typename Impl>
	inline void DestructorPtr(Impl* impl)
	{
		impl->~Impl();
	}

	// Initialize destructor function pointers.

	Buffer::DestructorFunc Buffer::s_destructor = DestructorPtr;
	Shader::DestructorFunc Shader::s_destructor = DestructorPtr;
	Material::DestructorFunc Material::s_destructor = DestructorPtr;
	Texture::DestructorFunc Texture::s_destructor = DestructorPtr;
	Framebuffer::DestructorFunc Framebuffer::s_destructor = DestructorPtr;
	

	// OpenGL

	static const UInt32 GLBufferTargets[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
	static const UInt32 GLBufferUsage[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
	static const UInt32 GLShaderTypes[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

	static const UInt32 GLEnables[] = {
		0,
		GL_BLEND,
		GL_DEPTH_TEST,
		GL_STENCIL_TEST,
		GL_SCISSOR_TEST,
	};

	template <typename Type>
	inline UInt32 GetIndex(Type clear, UInt32 offset)
	{
		Int32 index = static_cast<Int32>(clear)-offset;
		return index > 0 ? index : 0;
	}

	inline void GLEnable(bool status, UInt32 index)
	{
		if (index == 0)
		{
			return;
		}

		if (status)
		{
			glEnable(GLEnables[index]);
		}
		else
		{
			glDisable(GLEnables[index]);
		}
	}

	inline void CheckGL()
	{
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

	void GraphicsDevice::Enable(bool status, Features features)
	{
		GLEnable(status, GetIndex(features & Features::Blend, 0));
		GLEnable(status, GetIndex(features & Features::Depth, 0));
		GLEnable(status, GetIndex(features & Features::Stencil, 1));
		GLEnable(status, GetIndex(features & Features::Scissor, 4));
	}


	void GraphicsDevice::Clear(const Color32& color, ClearFlags clear)
	{ 
		static const UInt32 GLClears[] = {
			0,
			GL_COLOR_BUFFER_BIT,
			GL_DEPTH_BUFFER_BIT,
			GL_STENCIL_BUFFER_BIT,
		};
				
		glClearColor(color.r, color.g, color.b, color.a);
		const UInt32 flags = (0x4 & 0x4) - 1;
		glClear(GLClears[GetIndex(clear & ClearFlags::Color, 0)] | GLClears[GetIndex(clear & ClearFlags::Depth, 0)] | GLClears[GetIndex(clear & ClearFlags::Stencil, 1)]);
	}

	void GraphicsDevice::Present(Window& window)
	{
		SDL_GL_SwapWindow((*window)->sdlWindow);


	}

	void GraphicsDevice::Viewport(UInt32 w, UInt32 h)
	{
		ACE_ASSERT(w != 0, "Viewport width must be more than zero, %i", w);
		ACE_ASSERT(h != 0, "Viewport height must be more than zero, %i", h);

		glViewport(0, 0, w, h);
	}

	void GraphicsDevice::Scissor(Int32 x, Int32 y, UInt32 width, UInt32 height)
	{
		ACE_ASSERT(width != 0, "Scissor width must be more than zero, %i", width);
		ACE_ASSERT(height != 0, "Scissor height must be more than zero, %i", height);

		glScissor(x, y, width, height);
	}

	Buffer GraphicsDevice::CreateBuffer(BufferType type)
	{
		Buffer buffer(new BufferImpl(), type);
		return buffer;
	}

	void GraphicsDevice::BufferData(Buffer& buffer, UInt32 count, const Vertex* data, BufferUsage usage, UInt32 instances)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];
		buffer.size = count * (1 + instances);

		glBindBuffer(target, buffer->bufferID);

		if (instances > 0)
		{
			Vertex* instance = new Vertex[count * instances];

			for (UInt32 j = 0u; j < instances; ++j)
			{
				for (UInt32 i = 0u; i < count; ++i)
				{
					instance[i + j * count] = data[i];
					instance[i + j * count].position.w = (float)j;
				}
			}

			glBufferData(target, count * sizeof(Vertex) * instances, instance, GLBufferUsage[static_cast<UInt32>(usage)]);
			delete[] instance;
		}
		else
		{
			glBufferData(target, count * sizeof(Vertex), data, GLBufferUsage[static_cast<UInt32>(usage)]);

		}

		glBindBuffer(target, 0);

	}

	void GraphicsDevice::BufferSubData(Buffer& buffer, UInt32 count, UInt32 offset, const Vertex* data)
	{
		UInt32 target = GLBufferTargets[static_cast<UInt32>(buffer.type)];

		glBindBuffer(target, buffer->bufferID);
		glBufferSubData(target, offset, count * sizeof(Vertex), data);
		glBindBuffer(target, 0);
	}

	void GraphicsDevice::SetBuffer(const Buffer& buffer, BufferType type)
	{
			
		ACE_ASSERT(buffer, "Buffer is not initialized", "");


		UInt32 target = GLBufferTargets[static_cast<UInt32>(type)];
		glBindBuffer(target, buffer->bufferID);

		if (type == BufferType::Vertex)
		{
			glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)vertexAttributeSizes[0]);
			glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(vertexAttributeSizes[0] + vertexAttributeSizes[1]));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}
	}

	void GraphicsDevice::SetBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, buffer.type);
	}

	void GraphicsDevice::SetVertexBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, BufferType::Vertex);
	}

	void GraphicsDevice::SetIndexBuffer(const Buffer& buffer)
	{
		SetBuffer(buffer, BufferType::Index);
	}

	Texture GraphicsDevice::CreateTexture()
	{
		Texture texture(new TextureImpl());
		return texture;
	}

	void GraphicsDevice::UpdateTexture(Texture& texture, const UInt8* pixels, UInt32 w, UInt32 h, PixelFormat format)
	{
		ACE_ASSERT(w != 0, "Texture width must be more than zero, %i", w);
		ACE_ASSERT(h != 0, "Texture height must be more than zero, %i", h);


	

		#if ACE_WIN
			static const UInt32 GLFormat[] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH24_STENCIL8 };
			static const UInt32 GLFormatType[] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_FLOAT, GL_UNSIGNED_INT_24_8 };
		#else
            static const UInt32 GLFormat[] = { 0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH24_STENCIL8_OES };
            static const UInt32 GLFormatType[] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_FLOAT };
		#endif

		glBindTexture(GL_TEXTURE_2D, texture->textureID);
		
		UInt8 formatIndex = static_cast<UInt8>(format);
		glTexImage2D(GL_TEXTURE_2D, 0, GLFormat[formatIndex], w, h, 0, GLFormat[formatIndex], GLFormatType[formatIndex], pixels);

		SetTextureFlags(texture);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GraphicsDevice::SetTexture(Material& material, const Texture& texture, const char* name, UInt8 id)
	{
		//if (!material.textures[id])
		//{
		//	material.AddTexture(texture, name, id);
		//}

		ACE_ASSERT(material, "Material is not initialized.", "");
		//ACE_ASSERT(texture, "Texture is not initialized.", "");

		glUseProgram(material->materialID);
		glBindTexture(GL_TEXTURE_2D, texture->textureID);
		glActiveTexture(GL_TEXTURE0 + id);
		glUniform1i(glGetUniformLocation(material->materialID, name), id);
	}

	Shader GraphicsDevice::CreateShader(const char* source, ShaderType type)
	{
		ACE_ASSERT(source != nullptr, "Shader Source cannot be null", "");

		Shader shader(new ShaderImpl(GLShaderTypes[static_cast<UInt32>(type)]));
		shader.type = type;
		
		glShaderSource(shader->shaderID, 1, &source, NULL);
		glCompileShader(shader->shaderID);
		
		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;
		
		glGetShaderiv(shader->shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader->shaderID, GL_INFO_LOG_LENGTH, &errorMsgLength);
		
		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetShaderInfoLog(shader->shaderID, errorMsgLength, NULL, errorMsg);
			Logger::Log(Logger::Priority::Error, "%s", errorMsg);
		
			delete[] errorMsg;
		}

		return shader;
	}

	Material GraphicsDevice::CreateMaterial()
	{
		Material material(new MaterialImpl());
		return material;
	}

	Material GraphicsDevice::CreateMaterial(const Shader& vertex, const Shader& fragment)
	{
		Material material(new MaterialImpl());

		ACE_ASSERT(vertex, "Vertex shader is not initialized or valid.", "");
		ACE_ASSERT(fragment, "Fragment shader is not initialized or valid.", "");

		if (vertex)
		{
			glAttachShader(material->materialID, vertex->shaderID);
		}

		if (fragment)
		{
			glAttachShader(material->materialID, fragment->shaderID);
		}

		for (UInt32 i = 0; i < (UInt32)VertexAttributes::COUNT; ++i)
		{
			glBindAttribLocation(material->materialID, i, vertexAttributeNames[i]);
		}

		glLinkProgram(material->materialID);
	
		if (vertex)
		{
			glDetachShader(material->materialID, vertex->shaderID);
		}
		
		if (fragment)
		{
			glDetachShader(material->materialID, fragment->shaderID);
		}
		
		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;

		glGetProgramiv(material->materialID, GL_LINK_STATUS, &result);
		glGetProgramiv(material->materialID, GL_INFO_LOG_LENGTH, &errorMsgLength);

		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetProgramInfoLog(material->materialID, errorMsgLength, NULL, errorMsg);

			Logger::Log(Logger::Priority::Error, "%s", errorMsg);
			delete[] errorMsg;

			// TODO: Set default error material.
		}		
		return material;
	}

	void GraphicsDevice::Uniform(const Material& material, const char* name, const void* data, UniformType uniform, UInt32 elements)
	{
		glUseProgram(material->materialID);
		UInt32 location = glGetUniformLocation(material->materialID, name);

		switch (uniform)
		{
		case ace::UniformType::Int32:
			glUniform1iv(location, elements, static_cast<const Int32*>(data));
			break;
		case ace::UniformType::UInt32:
			glUniform1iv(location, elements, static_cast<const Int32*>(data));
			break;
		case ace::UniformType::Float:
			glUniform1fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Vec2:
			glUniform2fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Vec3:
			glUniform3fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Vec4:
			glUniform4fv(location, elements, static_cast<const float*>(data));
			break;
		case ace::UniformType::Mat2:
			glUniformMatrix2fv(location, elements, false, static_cast<const math::Matrix2*>(data)->array);
			break;
		case ace::UniformType::Mat3:
			glUniformMatrix3fv(location, elements, false, static_cast<const math::Matrix3*>(data)->array);
			break;
		case ace::UniformType::Mat4:
			glUniformMatrix4fv(location, elements, false, static_cast<const math::Matrix4*>(data)->array);
			break;
		}
		
		glUseProgram(0);

	}

	void GraphicsDevice::Draw(Material& material, UInt32 elements, UInt32 indicies, const UInt32* indexTable)
	{
		material.Apply();
		CheckGL();
		glUseProgram(material->materialID);

		SetMaterialFlags(material);

		if (indicies == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, elements);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, indicies, GL_UNSIGNED_INT, indexTable == nullptr ? 0 : indexTable);
		}

	}

	void GraphicsDevice::Draw(Material& material, const Mesh& mesh)
	{
		Enable(true, Features::Blend | Features::Depth);

		SetBuffer(mesh.vertexBuffer);
		SetBuffer(mesh.indexBuffer);

		Draw(material, mesh.GetElements(), mesh.GetIndicies());
	}

	void GraphicsDevice::Draw(Material& material, const Sprite& sprite)
	{
		static const UInt32 indexTable[] = {
			0,
			1,
			2,
			0,
			2,
			3,
		};

		static Buffer s_spriteBuffer = GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
		BufferData(s_spriteBuffer, 4, sprite.vertexData.data(), BufferUsage::Streaming);
		SetVertexBuffer(s_spriteBuffer);

		Draw(material, 0, 6, indexTable);
	}

	void GraphicsDevice::SetMaterialFlags(Material& material)
	{
		static const UInt32 GLBlendModes[] = {
			GL_ZERO, 
			GL_ONE,
			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,
		};

		static const UInt32 GLTestFlags[] = {
			GL_ALWAYS,
			GL_NEVER,
			GL_LESS,
			GL_EQUAL,
			GL_LEQUAL,
			GL_GREATER,
			GL_NOTEQUAL,
			GL_GEQUAL,
		};

		static const UInt32 GLCullingModes[] = {
			GL_BACK,
			GL_FRONT,
			GL_FRONT_AND_BACK
		};

		glBlendFunc(GLBlendModes[static_cast<UInt32>(material.flags.blendModesSrc)], GLBlendModes[static_cast<UInt32>(material.flags.blendModesDst)]);
		glDepthFunc(GLTestFlags[static_cast<UInt32>(material.flags.depthFlags)]);

		UInt32 culling = GL_BACK;
		
		if (material.flags.cullingMode == CullingMode::Both)
		{
			culling = 0;
		}
		else if (material.flags.cullingMode == CullingMode::Front)
		{
			culling = GL_FRONT;
		}

		if (culling != 0)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(culling);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		for (UInt8 i = 0; i < Material::MAX_TEXTURES; ++i)
		{
			if (material.textures[i])
			{
				SetTexture(material, material.textures[i], material.textureNames[i], i);
			}
		}
	}

	void GraphicsDevice::SetTextureFlags(Texture& texture)
	{
		static const UInt32 GLFilters[] = {
			GL_NEAREST,
			GL_LINEAR,
			GL_NEAREST_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR,
			GL_LINEAR_MIPMAP_LINEAR,
		};

		static const UInt32 GLWraps[] = {
			GL_REPEAT,
			GL_CLAMP_TO_EDGE
		};

		const UInt32 mag = static_cast<UInt32>(texture.flags.magFiltering);
		UInt32 min = static_cast<UInt32>(texture.flags.minFiltering);

		if (texture.flags.mipmaps)
		{
			if (min == 0 && mag == 0)
			{
				min = 2;
			}
			else if (min == 1 && mag == 0)
			{
				min = 3;
			}
			else if (min == 0 && mag == 1)
			{
				min = 4;
			}
			else if (min == 1 && mag == 1)
			{
				min = 5;
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLFilters[min]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLFilters[mag]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLWraps[static_cast<UInt32>(texture.flags.wrapModes)]);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLWraps[static_cast<UInt32>(texture.flags.wrapModes)]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GLWraps[static_cast<UInt32>(texture.flags.wrapModes)]);
	}

	Framebuffer GraphicsDevice::CreateFramebuffer()
	{
		Framebuffer framebuffer(new FramebufferImpl());
		
		return framebuffer;
	}

	void GraphicsDevice::SetFramebuffer(Framebuffer* framebuffer)
	{
		if (framebuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, (*framebuffer)->framebufferID);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void GraphicsDevice::SetFramebufferTarget(Framebuffer& framebuffer, Texture& texture, UInt32 framebufferAttachment)
	{
		ACE_ASSERT(framebuffer, "Framebuffer is not initialized.", "");
		ACE_ASSERT(texture, "Texture is not initialized.", "");

		static const UInt32 GLAttachments[] = {
			GL_DEPTH_ATTACHMENT, 
			GL_STENCIL_ATTACHMENT, 
			GL_COLOR_ATTACHMENT0
		};

		UInt32 attachments = framebufferAttachment < 3 ? GLAttachments[framebufferAttachment] : GLAttachments[3] + framebufferAttachment;

		SetFramebuffer(&framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments, GL_TEXTURE_2D, texture->textureID, 0);
		SetFramebuffer(nullptr);
	}
}
