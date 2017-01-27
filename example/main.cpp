#include <iostream>
#include <vector>

#include <Ace/Module.h>

#include <Ace/Window.h>
#include <Ace/Time.h>

#include <Ace/GraphicsDevice.h>


ace::Vertex triangle[3] = {
	{ { 0, 0.5f, 0 }, { 0, 0 }, 0xFF0000FFU },
	{ { -0.5f, 0, 0 }, { 1, 0 }, 0x0000FFFFU },
	{ { 0.5f, 0, 0 }, { 0, 1 }, 0x00FF00FFU },
};

int main(int, char**)
{
	ace::Init();

	ace::Window window("My Window", 480, 360);
	ace::GraphicsDevice::Viewport(480, 360);

	window.Clear(0xBADAFFFFU);

	ace::Buffer buffer = ace::GraphicsDevice::CreateBuffer(ace::BufferType::Vertex);
	ace::GraphicsDevice::BufferData(buffer, 3, triangle);

	ace::GraphicsDevice::SetVertexBuffer(buffer);
	ace::GraphicsDevice::Draw(3);

	window.Present();

	ace::Time::Delay(1000);

	return 0;
}