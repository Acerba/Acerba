#pragma once

#if defined(__ANDROID__) || defined(ANDROID)
	#define ACE_ANDROID 1
#elif defined(_WIN32)
	#define ACE_WIN 1
#endif

namespace ace
{
	// Functions for checking platform. (If needed)
}