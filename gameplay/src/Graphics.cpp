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

std::shared_ptr<Graphics::Renderer> Graphics::loadRenderer(std::string url)
{
    return nullptr;
}

const Graphics::Geometry& Graphics::Renderer::getGeometry() const
{
    return geometry;
}

size_t Graphics::Renderer::getMaterialCount() const
{
    return _materials.size();
}

const Graphics::Material& Graphics::Renderer::getMaterial(size_t index) const
{
    return _materials[index];
}

size_t Graphics::getVertexStride(Graphics::Format format)
{
   size_t result = 0;
    switch (format) 
    {
        case FORMAT_R8_UNORM: 
            result = 1; 
            break;
        case FORMAT_R16_UNORM: 
            result = 2; 
            break;
        case FORMAT_R16_FLOAT: 
            result = 2; 
            break;
        case FORMAT_R32_UINT: 
            result = 4; 
            break;
        case FORMAT_R32_FLOAT: 
            result = 4; 
            break;
        case FORMAT_R8G8_UNORM: 
            result = 2; 
            break;
        case FORMAT_R16G16_UNORM: 
            result = 4; 
            break;
        case FORMAT_R16G16_FLOAT: 
            result = 4; 
            break;
        case FORMAT_R32G32_UINT: 
            result = 8; 
            break;
        case FORMAT_R32G32_FLOAT: 
            result = 8; 
            break;
        case FORMAT_R8G8B8_UNORM: 
            result = 3; 
            break;
        case FORMAT_R16G16B16_UNORM: 
            result = 6; 
            break;
        case FORMAT_R16G16B16_FLOAT: 
            result = 6; 
            break;
        case FORMAT_R32G32B32_UINT: 
            result = 12; 
            break;
        case FORMAT_R32G32B32_FLOAT: 
            result = 12; 
            break;
        case FORMAT_B8G8R8A8_UNORM: 
            result = 4; 
            break;
        case FORMAT_R8G8B8A8_UNORM: 
            result = 4; 
            break;
        case FORMAT_R16G16B16A16_UNORM: 
            result = 8; 
            break;
        case FORMAT_R16G16B16A16_FLOAT:
            result = 8; 
            break;
        case FORMAT_R32G32B32A32_UINT: 
            result = 16; 
            break;
        case FORMAT_R32G32B32A32_FLOAT: 
            result = 16; 
            break;
    }
    return result;
}


}
