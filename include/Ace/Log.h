#pragma once

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

    };

	typedef Logger::Priority LogPriority;

}