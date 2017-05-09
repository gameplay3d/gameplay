#pragma once

#include "gameplay.h"

namespace gameplay
{

/**
 * Main application and game class.
 */
class App : public Game
{
public:

	/**
	 * Constructor.
	 */
	App();

	/**
	 * Destructor.
	 */
	~App();

	/**
	 * @see Game::onInitialize
	 */
	void onInitialize();

	/**
	 * @see Game::onFinalize
	 */
	void onFinalize();

    /**
     * @see Game::onSceneLoad
     */
    void onSceneLoad();

	/**
	 * @see Game::onResize
	 */
    void onResize(size_t width, size_t height);

	/**
	 * @see Game::onUpdate
	 */
    void onUpdate(float elapsedTime);

	/**
	 * @see Game::onUpdate
	 */
	void onRender(float elapsedTime);

 	/**
	 * @see Game::onGamepadEvent
	 */
    void onGamepadEvent(Platform::GamepadEvent evt, size_t gamepadIndex);

	/**
	 * @see Game::onKeyEvent
	 */
    void onKeyEvent(Platform::KeyboardEvent evt, int key);

	/**
	 * @see Game::onMouseEvent
	 */
    void onMouseEvent(Platform::MouseEvent evt, int x, int y, int wheelDelta);

	/**
	 * @see Game::onTouchEvent
	 */
	void onTouchEvent(Platform::TouchEvent evt, int x, int y, size_t touchIndex);
};

}