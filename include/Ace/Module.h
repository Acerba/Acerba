#pragma once

namespace ace
{
    /**
    @brief Initialize Acerba engine
    */
    void Init();

    /**
    @brief Initialize Acerba engine 
    */
    void Init(int argc, char** argv);

    /**
    @brief Updates all core modules: Audio, Event
    */
    void Update();

    /**
    @brief Shutdown Acerba engine
    */
    void Quit();
    
}