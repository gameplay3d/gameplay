#include "Base.h"
#include "Graphics.h"
#include "d3d12/GraphicsD3D12.h"
#include "vk/GraphicsVK.h"
#include "Game.h"

namespace gameplay
{

Graphics* Graphics::_graphics = nullptr;

Graphics* Graphics::getGraphics()
{
    if (!_graphics)
    {
        std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
        if (config->graphics.compare(GP_GRAPHICS_D3D12) == 0)
        {
            _graphics = new GraphicsD3D12();
            _graphics->_api = Graphics::API_D3D12;
        }
        else
        {
            _graphics = new GraphicsVK();
            _graphics->_api = Graphics::API_VK;
        }
    }
    return _graphics;
}

Graphics::Api Graphics::getApi()
{
    return _api;
}

}
