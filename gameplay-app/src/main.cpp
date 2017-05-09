#include "gameplay.h"

#if defined(_WINDOWS)
#include <windows.h>

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) 
{
	int argc = __argc;
	char** argv = __argv;

#elif defined (__linux__)

/**
 * Main application entry point.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 */
int main(int argc, char** argv) 
{

#endif
    gameplay::Platform* platform = gameplay::getPlatform();
    platform->startup(argc, argv);
    return platform->enterMessagePump();
}
