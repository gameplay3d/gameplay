#ifdef _WINDOWS

#pragma once

#include "Platform.h"
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <xinput.h>

#define WIN32_LEAN_AND_MEAN

namespace gameplay
{

/**
 * Windows impl of Platform
 * 
 * @see Platform
 */
class PlatformWindows : public Platform
{
public:

	/**
	 * Constructor
	 */
    PlatformWindows();

	/**
	 * Destructor
	 */
    ~PlatformWindows();

	/**
	 * @see Platform::startup
	 */
    bool startup(int argc, char** argv);

	/**
	 * @see Platform::shutdown
	 */
    void shutdown();

	/**
	 * @see Platform::enterMessagePump
	 */
    int enterMessagePump();

	/**
	 * @see Platform::getWindow
	 */
    unsigned long getWindow();

	/**
	 * @see Platform::getConnection
	 */
    unsigned long getConnection();


private:

    static LRESULT __stdcall onMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static KeyboardKey translateKey(WPARAM windowsKeyCode, bool shiftDown);
    static void updateCapture(LPARAM param);
    static void warpMouse(int clientX, int clientY);    

    HWND _hwnd;
    HINSTANCE _instance;
    Graphics* _graphics;
    XINPUT_STATE _gamepadState[4];
    bool _gamepadsConnected[4];
};

}

#endif
