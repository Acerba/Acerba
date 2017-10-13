#pragma once

#include <Ace/Platform.h>

namespace ace
{

    class Logger
    {

    public:

        enum class Priority
        {
            Debug,
            Info,
            Warning,
            Error
        };

        static void Log(const Priority priority, const char* message ...);

        static void LogError(const char* message ...);
        static void LogInfo(const char* message ...);

    #if ACE_DEBUG
        static void LogDebug(const char* message ...);
    #endif

    };

    typedef Logger::Priority LogPriority;

}