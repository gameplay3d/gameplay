#ifndef GAMEPLAYMAINWIN32_H_
#define GAMEPLAYMAINWIN32_H_

#ifdef WIN32

#include "gameplay.h"
using namespace gameplay;

#define GL_VERSION3
#include <GL/glew.h>
#include <GL/wglew.h>
#ifndef _WINDOWS_
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

/**
 * Main entry point.
 */
extern "C" int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
    Game* game = Game::getInstance();
    assert(game != NULL);
    Platform* platform = Platform::create(game);
    int result = platform->enterMessagePump();
    delete platform;
    return result;
}


#endif

#endif
