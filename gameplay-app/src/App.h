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
    void onInitialize(int argc, const char** argv);

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