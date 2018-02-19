#pragma once

#include <bx/pixelformat.h>
#include <bimg/bimg.h>
#include <bgfx/bgfx.h>

namespace gameplay
{

class Graphics
{
	friend class Game;

public:

	 /**
     * Defines the low-level graphics api.
     */
    enum API
    {
		API_NULL,
        API_DIRECT3D12,
		API_VULKAN,
        API_METAL
    };

	/**
	 * Constructor.
	 */
	Graphics();

	/**
	 * Destructor.
	 */
	~Graphics();

	/**
     * Gets the graphics api.
     *
     * @return The graphics api.
     */
    Graphics::API getAPI();
	
private:

    void onInitialize();
	void onFinalize();
    void onUpdate(float elapsedTime);
	void onPause();
	void onResume();

	Graphics::API _api;
	uint32_t _reset;
	uint32_t _debug;
};

}