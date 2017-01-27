#pragma once


//#include <SDL_opengles2.h>
//#include <gl/gl3w.h>
//#include <EGL/egl.h>
//#include <GLES2/gl2.h>

#if ACE_WIN

    #include <gl/gl3w.h>
    #include <SDL_opengl.h>

#else

#include <SDL_opengles2.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#endif
