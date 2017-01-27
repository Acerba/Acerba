#include <Ace/GraphicsDevice.h>
#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <Ace/Log.h>

#include <Ace/GL.h>
#include <Ace/GLBufferImpl.h>

namespace ace
{
	static const UInt32 GLBufferTargets[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};

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

	void GraphicsDevice::BufferData(Buffer& buffer, UInt32 count, Vertex* data)
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

	Buffer g_buffer;

	void GraphicsDevice::SetVertexBuffer(const Buffer& buffer)
	{
		g_buffer = buffer;

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

	void GraphicsDevice::Draw(UInt32 elements)
	{
		const char* vertex = " 					\n\
												\n\
		attribute vec3 position;				\n\
		attribute vec2 uv;						\n\
		attribute vec4 color;					\n\
												\n\
		varying vec4 o_c;						\n\
		varying vec2 o_uv;						\n\
												\n\
		void main()								\n\
		{										\n\
			o_c = color;						\n\
			o_uv = uv;							\n\
												\n\
			gl_Position = vec4(position.xyz, 1);\n\
		}";


		const char* fragment = "	\n\
									\n\
		varying lowp vec4 o_c;		\n\
		varying lowp vec2 o_uv;		\n\
									\n\
		void main()					\n\
		{							\n\
			gl_FragColor = o_c;		\n\
		}							\n\
			";

		UInt32 p = glCreateProgram();
		UInt32 v = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(v, 1, &vertex, NULL);
		glCompileShader(v);

		GLint result = GL_FALSE;
		GLint errorMsgLength = 0;

		glGetShaderiv(v, GL_COMPILE_STATUS, &result);
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &errorMsgLength);
		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetShaderInfoLog(v, errorMsgLength, NULL, errorMsg);
			Logger::Log(Logger::Priority::Info, "%s", errorMsg);

			delete[] errorMsg;
		}

		UInt32 f = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(f, 1, &fragment, NULL);
		glCompileShader(f);

		result = GL_FALSE;
		errorMsgLength = 0;

		glGetShaderiv(v, GL_COMPILE_STATUS, &result);
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &errorMsgLength);
		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetShaderInfoLog(v, errorMsgLength, NULL, errorMsg);
			Logger::Log(Logger::Priority::Info, "%s", errorMsg);

			delete[] errorMsg;
		}

		glAttachShader(p, v);
		glAttachShader(p, f);

		for (int i = 0; i < (int)VertexAttributes::COUNT; ++i)
		{
			glBindAttribLocation(p, i, vertexAttributeNames[i]);
		}

		glLinkProgram(p);


		result = GL_FALSE;
		errorMsgLength = 0;

		glGetProgramiv(p, GL_LINK_STATUS, &result);
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &errorMsgLength);

		if (errorMsgLength > 0)
		{
			char* errorMsg = new char[errorMsgLength + 1];
			glGetProgramInfoLog(p, errorMsgLength, NULL, errorMsg);

			Logger::Log(Logger::Priority::Info, "%s", errorMsg);
			delete[] errorMsg;

			// TODO: Set default error material.
		}

		SetVertexBuffer(g_buffer);
		glUseProgram(p);

		glDrawArrays(GL_TRIANGLES, 0, elements);
	}
}
