#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an abstract graphics system.
 */
class Graphics
{   
public:

    /**
     * Defines the low-level graphics api.
     */
    enum Api
    {
        API_VULKAN,
        API_DIRECT3D,
        API_METAL
    };

    class RenderContext
    {

    };
   
    /**
     * Gets the graphics system.
     *
     * @return The graphics system.
     */
    static Graphics* getGraphics();

    /**
     * Gets the graphics api.
     *
     * @return The graphics api.
     */
    static Graphics::Api getApi();

	/**
     * Called by platform or editor to initialize the graphics system.
	 *
	 * Exits application if fails.
	 *
	 * @param window The native window object/handle.
	 * @param connection The native connection to windowing system and application.
	 */
    virtual void initialize(unsigned long window, unsigned long connection) = 0;

    /**
     * Determines if the graphics system has completed initialization.
     *
     * @return true if the graphics system is initialized, false if not.
     */
    virtual bool isInitialized() = 0;

	/**
     * Called by platform or editor on window resize event
	 *
	 * @param width The width to resize to.
	 * @param height The width to resize to.
	 */
    virtual void onResize(int width, int height) = 0;

    /**
     * Determins if the graphics system have completed resizing.
     *
     * @return true if if the graphics system have completed resizing, false if not completed.
     */
    virtual bool isResized() = 0;

    /**
     * Gets the width of the graphics sytem presentation images.
     *
     * @return The width of the graphics sytem presentation images.
     */
    virtual int getWidth() = 0;

    /**
     * Gets the height of the graphics sytem presentation images.
     *
     * @return The height of the graphics sytem presentation images.
     */
    virtual int getHeight() = 0;    

    /**
     * Queues the presentation of swap images the platforms target surface.
     *
     * @param waitSemaphore The wait semaphore for presentation of swap images. 
     */
    virtual void present() = 0;

    /**
     * Waits idles while the pipeline flushes, releases resources and blocks until completed.
     */
    virtual void waitIdle() = 0;

    /**
     * Gets the render context.     
     */
    virtual std::shared_ptr<Graphics::RenderContext> getRenderContext() = 0;

    static Graphics* _graphics;
    static Graphics::Api _api;

};

}
