#include <Ace/Module.h>
#include <Ace/Window.h>
#include <Ace/Event.h>
#include <Ace/GraphicsDevice.h>

#include <Ace/Accelerometer.h>

#include <Ace/Log.h>

#include <SDL.h>

const ace::Vertex triangle[3] = {
	{{-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0xFF0000FFU},
	{{0.5f, 0.0f, 0.0f},  {0.0f, 0.0f}, 0x00FF00FFU},
	{{0.0f, 0.5f, 0.0f},  {0.0f, 0.0f}, 0x0000FFFFU},
};

int main(int argc, char** args)
{
	ace::Init();

	ace::Logger::Log(ace::Logger::Priority::Info, "Android Info!!");

	ace::Window window("Main", 480, 320);
	ace::Buffer buffer = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);

	ace::GraphicsDevice::BufferData(buffer, 3, triangle);

	while(true)
	{
		ace::Event::Update();

		window.Clear({0xBADAFFFFU});

		ace::GraphicsDevice::SetVertexBuffer(buffer);

		ace::Logger::Log(ace::Logger::Priority::Info, "Accelerometer: %f, %f", ace::Accelerometer::GetAxis(0), ace::Accelerometer::GetAxis(1));

		ace::GraphicsDevice::Draw(3);

		window.Present();
	}

	return 0;
}
