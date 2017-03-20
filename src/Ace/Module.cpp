
#include <Ace/Module.h>
#include <Ace/Audio.h>
#include <Ace/Event.h>
#include <SDL.h>

#include <iostream>

namespace ace
{

    class Module
    {

    private:

        //Disabled copy-ctors
        Module(const Module&) = delete;
        Module(const Module&&) = delete;
        Module operator=(const Module&) = delete;

    public:

        void Init()
        {
            #if ACE_WIN
                std::cout << "Acerba initialized" << std::endl;
            #endif
            SDL_Init(SDL_INIT_EVERYTHING);
        }

        void Init(int argc, char**)
        {

        }

        void Update()
        {
            Event::Update();
            Audio::Update();
        }

        void Quit()
        {
            #if ACE_WIN
                std::cout << "Acerba shut down" << std::endl;
            #endif
            SDL_Quit();
        }

        ///Initialize Acerba engine
        Module()
        {
            Init();
        }

        ///Shutdown Acerba engine
        ~Module()
        {

        }

    } module;


    void Init()
    {
        module.Init();
    }

    void Init(int argc, char** argv)
    {
        module.Init(argc, argv);
    }

	void Update()
	{
		module.Update();
	}

    void Quit()
    {
        module.Quit();
    }

}
