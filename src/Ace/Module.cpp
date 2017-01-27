
#include <Ace/Module.h>

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

        //TODO:
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

        void Quit()
        {
            std::cout << "Acerba shut down" << std::endl;
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

    void Quit()
    {
        module.Quit();
    }

}
