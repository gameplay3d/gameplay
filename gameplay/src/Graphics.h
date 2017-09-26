#pragma once

#include "VertexFormat.h"
#include "Buffer.h"
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
	 * Creates a vertex buffer.
	 *
	 * @param vertexFormat The format of the vertex data.
	 * @param vertexCount The number of vertices.
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @return The created vertex buffer.
	 */
	virtual std::shared_ptr<Buffer> createVertexBuffer(const VertexFormat& vertexFormat,  size_t vertexCount, bool hostVisible, void* hostMemory = nullptr) = 0;		
 	
	/**
	 * Creates a index buffer.
	 *
	 * @param indexFormat The format of the index data.
	 * @param indexCount The number of indices.
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @return The created index buffer.
	 */
	virtual std::shared_ptr<Buffer> createIndexBuffer(IndexFormat indexFormat,  size_t indexCount, bool hostVisible, void* hostMemory = nullptr) = 0;

	/**
	 * Creates a uniform buffer.
	 *
	 * @param size The size of the uniform buffer.
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @return The created uniform buffer.
	 */
	virtual std::shared_ptr<Buffer> createUniformBuffer(size_t size, bool hostVisible, void* hostMemory = nullptr) = 0;

	/**
	 * Destroys a buffer.
	 *
	 * @param buffer The buffer to be created.
	 */
	virtual void destroyBuffer(std::shared_ptr<Buffer> buffer) = 0;

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
