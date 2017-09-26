#pragma once

#include "VertexFormat.h"
#include "CommandList.h"

namespace gameplay
{

	class Mesh;

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
        API_VK,
        API_D3D12,
        API_MTL
    };

	/**
	 * Defines the primitive topology.
	 */
	enum PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_TRIANGLES,
        PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
        PRIMITIVE_TOPOLOGY_LINES,
        PRIMITIVE_TOPOLOGY_LINE_STRIP,
        PRIMITIVE_TOPOLOGY_POINTS
	};

	/**
     * Defines supported index formats.
     */
    enum IndexFormat
    {
        INDEX_FORMAT_UNSIGNED_SHORT,
        INDEX_FORMAT_UNSIGNED_INT
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
    Graphics::Api getApi();

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
	 * Creates a mesh that is a container for the vertex and index data.
	 *
	 * @param vertexFormat The format of the vertex data.
	 * @param vertexCount The number of vertices (not the size in bytes).
	 * @param dynamic true if you plan to dynamically update the mesh data at runtime, false if static.
	 * @return The mesh created.
	 */
	virtual std::shared_ptr<Mesh> createMesh(const VertexFormat& vertexFormat,  size_t vertexCount, bool dynamic = false) = 0;

	/**
	 * Creates a command list for processing gpu commands.
	 *
	 * @return The created command list for processing gpu commands.
	 */
	virtual std::shared_ptr<CommandList> createCommandList() = 0;

	/**
	 * Submits command lists for processing gpu commands.
	 *
	 * @param commandList The array of command lists to be submitted.
	 * @param count The number of commands lists to be submitted.
	 */
	virtual void submitCommandLists(std::shared_ptr<CommandList>* commandLists, size_t count) = 0;

    /**
     * Begin scene rendering.
	 *
	 * @return true if read to accept commands, false if fails.
     */
	virtual bool beginScene() = 0;

    /**
     * Ends scene rendering.
     */
    virtual void endScene() = 0;

    /**
     * Presents the contents of the framebuffer to the display.
     */
    virtual void present() = 0;

    /**
     * Flushes the commands in the queue, releases resources and blocks until completed.
     */
    virtual void flushAndWait() = 0;

    /**
     * Event occurs when the platform requests to initialize graphics.
	 *
	 * Exits application if fails.
	 *
	 * @param window The native window object/handle.
	 * @param connection The native connection to windowing system and application.
	 */
    virtual void onInitialize(unsigned long window, unsigned long connection) = 0;

    /**
     * Determines if the graphics system has completed initialization.
     *
     * @return true if the graphics system is initialized, false if not.
     */
    virtual bool isInitialized() = 0;

	/**
     * Event occurs when the platform window resizes.
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

    static Graphics* _graphics;
    Graphics::Api _api;

};

}
