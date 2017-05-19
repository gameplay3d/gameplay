#pragma once

namespace gameplay
{

typedef void* GraphicsCommmandBuffer;
typedef void* GraphicsRenderPass;
typedef void* GraphicsPipeline;
typedef void* GraphicsVertexFormat;
typedef void* GraphicsBuffer;
typedef void* GraphicsShader;
typedef void* GraphicsTexture;
typedef void* GraphicsConstant;

/**
 * Defines an abstract graphics system.
 *
 * The graphics system manages the gpu instance, surfaces, device, swapchains and queues.
 * Create command buffers and record graphics and compute commands into the command buffers.
 */
class Graphics
{   
public:

    /**
     * Defines the type of command processing queue.
     */
	enum QueueType
	{
		QUEUE_TYPE_DRAW,
		QUEUE_TYPE_COMPUTE
	};

    /**
     * The type of filtering.
     */
	enum FilterType
	{
		FILTER_TYPE_NEAREST,
		FILTER_TYPE_LINEAR
	};

    /**
     * The type of access when mapping memory.
     */
	enum MapAccess
	{
		MAP_FLAGS_BIT_READ,
		MAP_FLAGS_BIT_WRITE
	};

    /**
     * The type of topology for a primative.
     */
	enum PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_POINT_LIST,
		PRIMITIVE_TOPOLOGY_LINE_LIST,
		PRIMITIVE_TOPOLOGY_LINE_STRIP,
		PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
		PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
		PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
		PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
		PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
		PRIMITIVE_TOPOLOGY_PATCH_LIST
	};

    /**
     * Gets the graphics system.
     *
     * @return The graphics system.
     */
    static Graphics* getGraphics();

	/**
	 * Occurs when the platform initializes the graphics system.
	 *
	 * Exits application if fails.
	 *
	 * @param window The native window object/handle.
	 * @param connection The native connection to windowing system and application.
	 */
    virtual void initialize(unsigned long window, unsigned long connection = 0) = 0;

	/**
	 * Occurs when the platform resizes the graphics system.
	 *
	 * @param width The width to resize to.
	 * @param height The width to resize to.
	 */
    virtual void resize(int width, int height) = 0;

    virtual void render(float elapsedTime) = 0;

    virtual bool isInitialized() = 0;

	virtual bool isPrepared() = 0;

    virtual int getWidth() = 0;

    virtual int getHeight() = 0;

protected:

    static Graphics* _graphics;

};

}
