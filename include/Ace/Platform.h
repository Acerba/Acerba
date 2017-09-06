#pragma once

#if defined(__ANDROID__) || defined(ANDROID)
	#define ACE_ANDROID 1
	//#include <SDL_main.h>
#elif defined(_WIN32)
	#define ACE_WIN 1
#else
	#define ACE_LINUX 1
#endif

#if defined (_DEBUG)
    #define ACE_DEBUG 1
#endif

namespace ace
{
	// Functions for checking platform. (If needed)
}

// Android Hook

int AcerbaMain(int, char**);


#if ACE_ANDROID 
	#ifdef main
		#undef main
	#endif

	#define main AcerbaMain
#endif
