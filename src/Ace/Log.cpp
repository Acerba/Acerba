#include <Ace/Log.h>
#include <Ace/IntTypes.h>

#include <SDL_log.h>
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

    void Logger::Log(Priority priority, const char* message ...)
    {

		static const UInt32 offset = 1;

		UInt32 prioritySize = strlen(Priorities[static_cast<UInt32>(priority)]);
		UInt32 formatSize = strlen(message);

		char* format = new char[formatSize + offset + prioritySize + 1];
		strncpy(format, Priorities[static_cast<UInt32>(priority)], prioritySize);
		strncpy(format + offset + prioritySize, message, formatSize);

		format[prioritySize] = ' ';
		format[prioritySize + offset + formatSize] = '\0';

		va_list args;
		va_start(args, message);
		//SDL_LOG_CATEGORY_APPLICATION does not print debug or verbose messages
		SDL_LogMessageV(SDL_LOG_CATEGORY_TEST, SDL_Priorities[static_cast<UInt32>(priority)], format, args);
		va_end(args);

		delete[] format;
    }
}