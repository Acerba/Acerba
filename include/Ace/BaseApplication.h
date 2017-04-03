#pragma once
#include <Ace/Window.h>
namespace ace
{
	class BaseApplication
	{
	public:
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Load() = 0;
		virtual void Quit() = 0;
	
		BaseApplication(const char* title = "Demo", UInt16 width = 1280, UInt16 height = 720);
	protected:
		Window m_window;
		void GameLoop();
	};
}