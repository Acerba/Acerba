#include <Ace/Module.h>
#include <Ace/Window.h>
#include <Ace/Event.h>
#include <Ace/GraphicsDevice.h>

#include <Ace/Accelerometer.h>

#include <Ace/Log.h>

#include <SDL.h>

const ace::Vertex triangle[3] = {
	{ { -0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f }, 0xFF0000FFU },
	{ { 0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f }, 0x00FF00FFU },
	{ { 0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f }, 0x0000FFFFU },
};

int main(int, char**)
{
	ace::Init();

	ace::Logger::Log(ace::Logger::Priority::Info, "Hello!");
	ace::Window window("Main", 480, 320);

	ace::Buffer buffer = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
	ace::GraphicsDevice::BufferData(buffer, 3, triangle);

	ace::Shader vertex, fragment;

	vertex.Init("									\n\
		#version 100								\n\
		attribute vec3 a_position;					\n\
		attribute vec2 a_uv;						\n\
		attribute vec4 a_color;						\n\
													\n\
		varying vec4 o_c;							\n\
		varying vec2 o_uv;							\n\
													\n\
		void main()									\n\
		{											\n\
			o_c = a_color;							\n\
			o_uv = a_uv;							\n\
													\n\
			gl_Position = vec4(a_position.xyz, 1);	\n\
		}											\n\
		", ace::ShaderType::Vertex);

	fragment.Init("									\n\
		#version 100								\n\
		varying lowp vec4 o_c;						\n\
		varying lowp vec2 o_uv;						\n\
													\n\
		void main()									\n\
		{											\n\
			gl_FragColor = o_c;						\n\
		}											\n\
		", ace::ShaderType::Fragment);

	ace::Material material(vertex, fragment);


	while(true)
	{
		window.Clear(0xBADAFFFFU);

		ace::GraphicsDevice::SetVertexBuffer(buffer);
		ace::GraphicsDevice::Draw(material, 3);

		window.Present();

	}



	return 0;
}
