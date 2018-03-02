#include "Base.h"
#include "Graphics.h"
#include "Game.h"

namespace gameplay
{

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::onInitialize()
{
}

void Graphics::onFinalize()
{
}

void Graphics::onResize(size_t width, size_t height)
{
}

void Graphics::onRender(float elapsedTime)
{
    Game* game = Game::getInstance();
    std::shared_ptr<SceneObject> SceneObject = game->getScene();
}

void Graphics::onPause()
{
}

void Graphics::onResume()
{
}

}
