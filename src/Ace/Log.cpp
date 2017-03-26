#include <Ace/Log.h>
#include <Ace/IntTypes.h>

#include <SDL_log.h>
#include <SDL_assert.h>
#include <string.h>

namespace ace
{

    static const SDL_LogPriority SDL_Priorities[] = {
        SDL_LOG_PRIORITY_DEBUG,
        SDL_LOG_PRIORITY_INFO,
        SDL_LOG_PRIORITY_WARN,
        SDL_LOG_PRIORITY_ERROR
    };

    static const char* Priorities[] = {
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
    };


    void _print(Logger::Priority priority, const char* message, va_list args)
    {
        static const UInt32 offset = 1u;

        UInt32 prioritySize = strlen(Priorities[static_cast<UInt32>(priority)]);
        UInt32 formatSize = strlen(message);

        char* format = new char[formatSize + offset + prioritySize + 1];
        strncpy(format, Priorities[static_cast<UInt32>(priority)], prioritySize);
        strncpy(format + offset + prioritySize, message, formatSize);

        format[prioritySize] = ' ';
        format[prioritySize + offset + formatSize] = '\0';

        //SDL_LOG_CATEGORY_APPLICATION does not print debug or verbose messages
        SDL_LogMessageV(SDL_LOG_CATEGORY_TEST, SDL_Priorities[static_cast<UInt32>(priority)], format, args);

        delete[] format;

		if (priority == Logger::Priority::Error)
		{
			SDL_assert(false && "Acerba Error");
		}
    }


    void Logger::Log(Priority priority, const char* message ...)
    {
        va_list args;
        va_start(args, message);
        _print(priority, message, args);
        va_end(args);
    }

    void Logger::LogError(const char* message ...)
    {
        va_list args;
        va_start(args, message);
        _print(Priority::Error, message, args);
        va_end(args);
    }

    void Logger::LogInfo(const char* message ...)
    {
        va_list args;
        va_start(args, message);
        _print(Priority::Info, message, args);
        va_end(args);
    }
}