#include <Ace/BaseApplication.h>
#include <SDL.h>
#include <Ace/Audio.h>
#include <Ace/Event.h>
#include <Ace/Module.h>
namespace ace
{
	BaseApplication::BaseApplication(const char* title, UInt16 w, UInt16 h) : m_window(title, w, h)
	{

	}

	void BaseApplication::GameLoop()
	{
		ace::Init();
		Init();
		Load();
		while (m_window)
		{
			ace::Update();
			Update();
			Draw();
		}
		Quit();
		ace::Quit();
	}
}