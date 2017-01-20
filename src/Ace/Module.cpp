
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
        void Init(int argc, char**)
        {

        }

        void Quit()
        {

        }

        ///Initialize Acerba engine
        Module()
        {
            std::cout << "Acerba initialized" << std::endl;
            SDL_Init(SDL_INIT_EVERYTHING);
        }

        ///Shutdown Acerba engine
        ~Module()
        {
            std::cout << "Acerba shut down" << std::endl;
            SDL_Quit();
        }

    } module;


    void Init()
    {
        
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