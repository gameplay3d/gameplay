#pragma once

#include "gameplay.h"

namespace gameplay
{

/**
 * Main application and game class.
 */
class Editor : public Game
{
public:

	/**
	 * Constructor.
	 */
	Editor();

	/**
	 * Destructor.
	 */
	~Editor();

	/**
	 * @see Game::onInitialize
	 */
	void onInitialize(int argc, const char* const* argv);

	/**
	 * @see Game::onFinalize
	 */
	void onFinalize();

	/**
	 * @see Game::onResize
	 */
    void onResize(size_t width, size_t height);

	/**
	 * @see Game::onUpdate
	 */
    void onUpdate(float elapsedTime);

private:

	// TODO:

};

}
