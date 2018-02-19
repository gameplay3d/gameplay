#include "Base.h"
#include "Graphics.h"
#include "Game.h"
#include "Platform.h"

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/pixelformat.h>
#include <bimg/bimg.h>
#include <bgfx/bgfx.h>

namespace gameplay
{

Graphics::Graphics() :
	_api(API::API_NULL),
	_reset(BGFX_RESET_VSYNC),
	_debug(BGFX_DEBUG_NONE)
{
}

Graphics::~Graphics()
{
}

Graphics::API Graphics::getAPI()
{
	return _api;
}

void Graphics::onInitialize()
{
	_api = API::API_NULL;
	bgfx::RendererType::Enum rendererType = bgfx::RendererType::Noop;
	std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
	if (config->graphics.compare(GP_GRAPHICS_DIRECT3D12) == 0)
	{
		_api = API::API_DIRECT3D12;
		rendererType = bgfx::RendererType::Direct3D12;
	}
	else if (config->graphics.compare(GP_GRAPHICS_VULKAN) == 0)
	{
		_api = API::API_VULKAN;
		rendererType = bgfx::RendererType::Vulkan;
	}
	else if (config->graphics.compare(GP_GRAPHICS_METAL) == 0)
	{
		_api = API::API_METAL;
		rendererType = bgfx::RendererType::Metal;
	}
	
	bgfx::init(rendererType, 0);
	_reset = BGFX_RESET_VSYNC;
	bgfx::reset(config->width, config->height, _reset);

	_debug = BGFX_DEBUG_TEXT;
	bgfx::setDebug(_debug);
}

void Graphics::onFinalize()
{
	bgfx::shutdown();
}

void Graphics::onUpdate(float elapsedTime)
{
}

void Graphics::onPause()
{
}

void Graphics::onResume()
{
}

}