#include <Ace/Module.h>

#include <Ace/Audio.h>
#include <Ace/Animation.h>
#include <Ace/EntityManager.h>
#include <Ace/Event.h>
#include <Ace/Time.h>
#include <Ace/Platform.h>
#include <Ace/Camera.h>

#include <SDL.h>


namespace ace
{
    void Pause(bool);

    int SDLPause(void* userdata, SDL_Event* event)
    {
        // Handles android pause
        switch (event->type)
        {
        case SDL_APP_WILLENTERBACKGROUND:
        case SDL_APP_DIDENTERBACKGROUND:
        case SDL_APP_WILLENTERFOREGROUND:
        case SDL_APP_DIDENTERFOREGROUND:

            Pause(true);
            return 0;
        }

        return 1;
    }


    class Module
    {

    private:

        //Disabled copy-ctors
        Module(const Module&) = delete;
        Module(const Module&&) = delete;
        Module operator=(const Module&) = delete;

        bool m_isInitialized;
        bool m_isPaused, m_lastPauseState;

    public:

        void Init()
        {
            if (!m_isInitialized)
            {
                SDL_Init(SDL_INIT_EVERYTHING);
                Audio::Init();
                m_isInitialized = true;
                m_isPaused = false;

                SDL_SetEventFilter(SDLPause, nullptr);
            }
        }

        void Init(int, char**)
        {
            if (!m_isInitialized)
            {
                SDL_Init(SDL_INIT_EVERYTHING);
                Audio::Init();
                m_isInitialized = true;
                m_isPaused = false;

                SDL_SetEventFilter(SDLPause, nullptr);
            }
        }

        void Update()
        {
            m_lastPauseState = m_isPaused;

            do
            {
                Event::Update();
            } while (m_isPaused);


            if (m_lastPauseState != m_isPaused)
            {
                // Resets delta time.
                Time::Update();
            }

            Time::Update();
            Animation::Update();
            EntityManager::Update();
            Camera::UpdateMainCamera();

        }

        void Pause(bool status)
        {
            m_isPaused = status;
        }

        void Quit()
        {
            if (m_isInitialized)
            {
                Audio::Quit();
                SDL_Quit();
                m_isInitialized = false;
            }
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

    void Pause(bool status)
    {
        module.Pause(status);
    }

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