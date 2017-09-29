#include <Ace/Module.h>

#include <Ace/Audio.h>
#include <Ace/EntityManager.h>
#include <Ace/Event.h>
#include <Ace/Time.h>
#include <Ace/Platform.h>
#include <Ace/Camera.h>

#include <SDL.h>


namespace ace
{
    class Module
    {

    private:

        //Disabled copy-ctors
        Module(const Module&) = delete;
        Module(const Module&&) = delete;
        Module operator=(const Module&) = delete;

		bool m_isInitialized;

    public:

        void Init()
        {
			if (m_isInitialized)
			{
				return;
			}

            SDL_Init(SDL_INIT_EVERYTHING);
			Audio::Init();

			m_isInitialized = true;
        }

        void Init(int argc, char**)
        {

        }

        void Update()
        {
            Event::Update();
			Time::Update();
			EntityManager::Update();
			Camera::UpdateMainCamera();
        }

        void Quit()
        {
			if (!m_isInitialized)
			{
				return;
			}

			Audio::Quit();
            SDL_Quit();

			m_isInitialized = false;
        }

        ///Initialize Acerba engine
		Module() : m_isInitialized(false)
        {
			#if ACE_WIN
				Init();
			#endif
        }

        ///Shutdown Acerba engine
        ~Module()
        {
			Quit();
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