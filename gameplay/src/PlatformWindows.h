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

	/**
	 * @see Platform::isGamepadButtonPressed
	 */
	bool isGamepadButtonPressed(GamepadButton button, size_t gamepadIndex);

	/**
	 * @see Platform::getGamepadAxisValues
	 */
	void getGamepadAxisValues(float* leftVertical, float* leftHorizontal, float* rightVertical, float* rightHorizontal, size_t gamepadIndex);

	/**
	 * @see Platform::getGamepadTriggerValues
	 */
	void getGamepadTriggerValues(float* leftTrigger, float* rightTrigger, size_t gamepadIndex);

	/**
	 * @see Platform::getAccelerometerValues
	 */
	void getAccelerometerValues(float* pitch, float* roll);

	/**
	 * @see Platform::getSensorValues
	 */
	void getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ);

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
