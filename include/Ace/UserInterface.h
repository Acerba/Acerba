#pragma once

#include <Ace/Event.h>
#include <Ace/Types.h>


namespace ace
{

    class Window;

    //struct SDL_Window;
    //typedef union SDL_Event SDL_Event;


    class UserInterface
    {

        static bool m_debugEnabled;

        UserInterface() = delete;
        ~UserInterface() = delete;
        UserInterface(UserInterface&&) = delete;
        UserInterface(const UserInterface&) = delete;
        UserInterface& operator=(const UserInterface&) = delete;

        static void MakeDebug();

    public:


        static void Begin();

        static void End();

        static void Init(const Window& window);

        static void SetDebug(const bool status);

        static void Quit();

    };


    typedef UserInterface UI;



}