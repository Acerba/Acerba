#pragma once

#include <Ace/Platform.h>

#if ACE_DEBUG

    #include <sstream>
    #include <string>

    #if ACE_LINUX
        #include <iostream>
    #endif

#endif

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

            template <typename T>
            static std::string ToString(T& t)
            {
                std::stringstream ss;
                ss << t;
                return ss.str();
            }

            template <typename T, typename ... Args>
            static std::string ToString(T& t, Args& ... args)
            {
                return ToString(t) + ToString(args...);
            }

            template <typename ... Args>
            static void LogDebug(Args& ... args)
            {
                #if ACE_LINUX
                    std::cout << ToString(args...) << '\n';
                #endif
                //Log(Priority::Debug, ToString(args...).c_str());
            }

        #endif

    };

	typedef Logger::Priority LogPriority;

}