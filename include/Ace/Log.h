#pragma once

namespace ace
{

    class Logger
    {

    public:

        enum class PRIORITY
        {
            Debug,
            Info,
            Warning,
            Error
        };

        static void Log(const PRIORITY priority, const char* message ...);

    };

}