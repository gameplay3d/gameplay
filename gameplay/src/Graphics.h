#pragma once

#include "VertexFormat.h"
#include "Buffer.h"
#include "CommandPool.h"
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
	virtual std::shared_ptr<Buffer> createVertexBuffer(const VertexFormat& vertexFormat,  size_t vertexCount, bool hostVisible) = 0;		
 	
	/**
	 * Creates a index buffer.
	 *
	 * @param indexFormat The format of the index data.
	 * @param indexCount The number of indices.
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @return The created index buffer.
	 */
	virtual std::shared_ptr<Buffer> createIndexBuffer(IndexFormat indexFormat,  size_t indexCount, bool hostVisible) = 0;

	/**
	 * Creates a uniform buffer.
	 *
	 * @param size The size of the uniform buffer.
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @return The created uniform buffer.
	 */
	virtual std::shared_ptr<Buffer> createUniformBuffer(size_t size, bool hostVisible) = 0;

	/**
	 * Destroys a buffer.
	 *
	 * @param buffer The buffer to be created.
	 */
	virtual void destroyBuffer(std::shared_ptr<Buffer> buffer) = 0;

	/**
	 * Creates a command pool for processing gpu commands buffer.
	 *
	 * @param transient Indicates the command from this pool are short-lived, false if not short-lived.
	 * @return The created command list for processing gpu commands.
	 */
	virtual std::shared_ptr<CommandPool> createCommandPool(bool transient = false) = 0;

	/**
	 * Destroy a command pool.
	 *
     * @param pool The command pool to detroy.
	 */
	virtual void destroyCommandPool(std::shared_ptr<CommandPool> pool) = 0;

	/**
	 * Creates a command list that is used for encoding command that are submitted to the gpu.
	 *
	 * @param pool The command pool this list is created from.
	 * @param secondary true if this is a secondary command list with lower processing priority, false if primary.
	 * @return The created commandlist.
	 */
	virtual std::shared_ptr<CommandList> createCommandList(std::shared_ptr<CommandPool> pool, bool secondary) = 0;

	/**
	 * Creates an array command list that is used for encoding command that are submitted to the gpu.
	 *
	 * @param pool The command pool this list is created from.
	 * @param secondary true if this is a secondary command list with lower processing priority, false if primary.
	 * @param count The number of command list to be created.
	 * @param out The vector of command lists to be added into.
	 */
	virtual void createCommandLists(std::shared_ptr<CommandPool> pool, bool secondary, size_t count,
									std::vector<std::shared_ptr<CommandList>> out) = 0;
	/**
	 * Destroys a command list.
	 *
	 * @param commandList The command list to be destroyed.
	 */
	virtual void destroyCommandList(std::shared_ptr<CommandList> commandList) = 0;

	/**
	 * Destroys the command lists.
	 *
	 * @param commandLists The vector of command lists to be destroyed.
	 */
	virtual void destroyCommandLists(std::vector<std::shared_ptr<CommandList>> commandLists) = 0;

	/**
	 * Submits a vector command lists for processing the gpu commands in them.
	 *
	 * @param commands The command list to be submitted.
	 */
	virtual void submitCommands(std::shared_ptr<CommandList> commands) = 0;

	/**
	 * Submits a vector command lists for processing the gpu commands in them.
	 *
	 * @param commands The command lists to be submitted.
	 */
	virtual void submitCommands(std::vector<std::shared_ptr<CommandList>> commands) = 0;

    /**
     * Flushes the commands in the queue, releases resources and blocks until completed.
     */
    virtual void flushCommands() = 0;

    /**
     * Presents the contents of the framebuffer to the display.
     */
    virtual void present() = 0;

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
