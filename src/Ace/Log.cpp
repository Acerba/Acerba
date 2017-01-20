#include <Ace/Log.h>
#include <Ace/IntTypes.h>

#include <SDL_log.h>

namespace ace
{

    static const SDL_LogPriority SDL_Priorities[] = {
        SDL_LOG_PRIORITY_DEBUG,
        SDL_LOG_PRIORITY_INFO,
        SDL_LOG_PRIORITY_WARN,
        SDL_LOG_PRIORITY_ERROR
    };

    void Logger::Log(PRIORITY priority, const char* message ...)
    {
        va_list args;
        va_start(args, message);
        //SDL_LOG_CATEGORY_APPLICATION does not print debug or verbose messages
        SDL_LogMessageV(SDL_LOG_CATEGORY_TEST, SDL_Priorities[static_cast<UInt32>(priority)], message, args);
        va_end(args);
    }


}