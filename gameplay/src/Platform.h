#pragma once

#include "Input.h"

#define SDL_MAIN_HANDLED
#include <sdl/SDL.h>
#include <sdl/SDL_syswm.h>

namespace gameplay
{

/**
 * Defines a class for platform windowing and events.
 *
 * This class is only run from the main entry point.
 * It delegates useful event needed by the game engine 
 * classes such as Game, Input and UI.
 */
class Platform
{
public:

	/**
	 * Gets the platform.
	 */
	static Platform* getPlatform();

	/**
	 * Runs the platform and polls for events.
	 *
	 * @return The returned exit code.
	 */
	int run();

	/**
	 * Gets the width of the platform window.
	 *
	 * @return The width of the platform window.
	 */
	size_t getWidth() const;

	/**
	 * Gets the height of the platform window.
	 *
	 * @return The height of the platform window.
	 */
	size_t getHeight() const;

	/**
	 * Gets the height of the platform window.
	 *
	 * @return The height of the platform window.
	 */
	bool isFullscreen() const;

	/**
	 * Gets the pointer to the native display or instance.
	 *
	 * @return The pointer to the native display or instance.
	 */
	void* getNativeDisplay() const;

	/**
	 * Gets the pointer to the native window or view.
	 *
	 * @return The pointer to the native window or view.
	 */
	void* getNativeWindow() const;

private:

	Platform();
	~Platform();
	void initTranslateKey(uint16_t sdl, Input::Key key);
	Input::Key translateKey(SDL_Scancode sdl);
	uint8_t translateKeyModifiers(uint16_t sdl);
	uint8_t translateKeyModifierPress(uint16_t key);
	void initTranslateGamepad(uint8_t sdl, Input::Key button);
	Input::Key translateGamepad(uint8_t sdl);
	void initTranslateGamepadAxis(uint8_t sdl, Input::GamepadAxis axis);
	Input::GamepadAxis translateGamepadAxis(uint8_t sdl);

	SDL_Window* _window;
	size_t _width;
	size_t _height;
	bool _fullscreen;
	void* _nativeDisplay;
	void* _nativeWindow;
	bool _running;
	uint8_t _translateKey[256];
	uint8_t _translateGamepad[256];
	uint8_t _translateGamepadAxis[256];
};

}
