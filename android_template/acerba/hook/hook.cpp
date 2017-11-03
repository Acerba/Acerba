#include <SDL_main.h>

#include <Ace/Audio.h>
#include <Ace/Log.h>

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>

extern int AcerbaMain(int, char**);

int SDL_main(int argc, char** args)
{
	// INIT

		cOAL_Init_Params oal_parms;

		if (OAL_Init(oal_parms) == false)
		{
			ace::Logger::LogInfo("Audio initializing failed!");
		}
		else
		{
			ace::Logger::LogInfo("Audio initializing succeeded!");
		}

		ace::Audio::Init(true);
		
	int result = AcerbaMain(argc, args);

	// QUIT
	
	return result;
}