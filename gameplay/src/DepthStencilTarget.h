#ifndef DEPTHSTENCILTARGET_H_
#define DEPTHSTENCILTARGET_H_

#include "Base.h"
#include "Texture.h"

namespace gameplay
{

/**
 * Defines a contain for depth and stencil targets in a frame buffer object.
 *
 * This class assumes that the target hardware supports depth textures, since
 * creation of a DepthStencilTarget always attempts to create an underlying
 * depth texture.
 */
class DepthStencilTarget : public Ref
{
    friend class FrameBuffer;

public:

    /**
     * Defines the accepted formats for DepthStencilTargets.
     */
    enum Format
    {
        /**
         * A target with depth data.
         */
        DEPTH,

        /**
         * A target with depth data and stencil data.
         */
        DEPTH_STENCIL
    };

    /**
     * Create a DepthStencilTarget and add it to the list of available DepthStencilTargets.
     *
     * @param id The ID of the new DepthStencilTarget.  Uniqueness is recommended but not enforced.
     * @param format The format of the new DepthStencilTarget.
     * @param width Width of the new DepthStencilTarget.
     * @param height Height of the new DepthStencilTarget.
     *
     * @return A newly created DepthStencilTarget.
     */
    static DepthStencilTarget* create(const char* id, Format format, unsigned int width, unsigned int height);

    /**
     * Get a named DepthStencilTarget from its ID.
     *
     * @param id The ID of the DepthStencilTarget to search for.
     *
     * @return The DepthStencilTarget with the specified ID, or NULL if one was not found.
     */
    static DepthStencilTarget* getDepthStencilTarget(const char* id);

    /**
     * Get the ID of this DepthStencilTarget.
     *
     * @return The ID of this DepthStencilTarget.
     */
    const char* getId() const;

    /**
     * Returns the format of the DepthStencilTarget.
     *
     * @return The format.
     */
    Format getFormat() const;

private:

    /**
     * Constructor.
     */
    DepthStencilTarget(const char* id, Format format);

    /**
     * Destructor.
     */
    ~DepthStencilTarget();

    std::string _id;
    Format _format;
    RenderBufferHandle _renderBuffer;
};

}

#endif
