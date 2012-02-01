#ifdef WIN32

#include "gameplay.h"

using namespace gameplay;

#ifndef _WINDOWS_
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

extern const int WINDOW_WIDTH  = 1024;
extern const int WINDOW_HEIGHT = 600;

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
