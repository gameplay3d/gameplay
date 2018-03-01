#include "App.h"

namespace gameplay
{

App app;

App::App()
{
}

App::~App()
{
}

void App::onInitialize(int argc, const char** argv)
{
    Game::onInitialize(argc, argv);
    
    // TODO:
}

void App::onFinalize()
{
    // TODO:

    Game::onFinalize();
}

void App::onResize(size_t width, size_t height)
{
    Game::onResize(width, height);

    // TODO:
}

void App::onUpdate(float elapsedTime)
{
    Game::onUpdate(elapsedTime);

    // TODO:
}



}
