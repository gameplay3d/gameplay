#include "Base.h"
#include "Graphics.h"
#include "GraphicsDirect3D.h"
#include "GraphicsVulkan.h"
#include "Game.h"

namespace gameplay
{

Graphics* Graphics::_graphics = nullptr;

Graphics* Graphics::getGraphics()
{
    if (!_graphics)
    {
        std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
        if (config->graphics.compare(GP_GRAPHICS_DIRECT3D) == 0)
        {
            _graphics = new GraphicsDirect3D();
        }
        else
        {
            _graphics = new GraphicsVulkan();
        }
    }
    return _graphics;
}


}
