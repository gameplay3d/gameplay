#include "Base.h"
#include "Graphics.h"
#include "GraphicsDirect3D.h"
#include "GraphicsVulkan.h"
#include "Game.h"

namespace gameplay
{

Graphics* Graphics::_graphics = nullptr;
Graphics::Api Graphics::_api = Graphics::API_VULKAN;

Graphics* Graphics::getGraphics()
{
    if (!_graphics)
    {
        std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
        if (config->graphics.compare(GP_GRAPHICS_DIRECT3D) == 0)
        {
            _graphics = new GraphicsDirect3D();
            _graphics->_api = Graphics::API_DIRECT3D;
        }
        else
        {
            _graphics = new GraphicsVulkan();
            _graphics->_api = Graphics::API_VULKAN;
        }
    }
    return _graphics;
}

Graphics::Api Graphics::getApi()
{
    return _api;
}

}
