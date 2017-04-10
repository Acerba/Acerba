#pragma once

#include <Ace/Platform.h>
#include <Ace/Log.h>
#include <SDL_assert.h>

#if defined (ACE_DEBUG)
#define ACE_ASSERT(condition, format, expression) if(static_cast<bool>(condition) == false){ ace::Logger::LogError(format, expression); SDL_assert(condition && format); }
#else
    #define ACE_ASSERT(condition, format, expression)
#endif
