#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_

#include "Base.h"
#include "Texture.h"

namespace gameplay
{

/**
 * Defines a linear area of display memory and usually resides 
 * in the display memory of the graphics device.
 */
class RenderTarget : public Ref
{
    friend class FrameBuffer;

public:
 
    /**
     * Create a RenderTarget and add it to the list of available RenderTargets.
     *
     * The created RenderTarget contains a 32-bit texture with a single/base mipmap level only.
     *
     * @param id The ID of the new RenderTarget.
     * @param width The width of the new RenderTarget.
     * @param height The height of the new RenderTarget.
     *
     * @return A newly created RenderTarget.
     * @script{create}
     */
    static RenderTarget* create(const char* id, unsigned int width, unsigned int height, Texture::Format format = Texture::RGBA);

    /**
     * Create a RenderTarget from the given Texture and add it to the list of
     * available RenderTargets.
     *
     * Note that different hardware and OpenGL versions have different capabilities
     * and restrictions on what texture formats are supported as render targets.
     *
     * @param id The ID of the new RenderTarget.
     * @param texture The texture for the new RenderTarget.
     *
     * @return A newly created RenderTarget.
     * @script{create}
     */
    static RenderTarget* create(const char* id, Texture* texture);

    /**
     * Get a named RenderTarget from its ID.
     *
     * @param id The ID of the RenderTarget to search for.
     *
     * @return The RenderTarget with the specified ID, or NULL if one was not found.
     */
    static RenderTarget* getRenderTarget(const char* id);

    /**
     * Get the ID of this RenderTarget.
     *
     * @return The ID of this RenderTarget.
     */
    const char* getId() const;

    /**
     * Get the backing texture of this RenderTarget.
     *
     * @return The backing texture of this RenderTarget.
     */
    Texture* getTexture() const;

    /**
     * Returns the width of the RenderTarget.
     *
     * @return The width.
     */
    unsigned int getWidth() const;

    /**
     * Returns the height of the RenderTarget.
     *
     * @return The height.
     */
    unsigned int getHeight() const;
 
private:

    /**
     * Constructor.
     */
    RenderTarget(const char* id);

    /**
     * Destructor.
     */
    ~RenderTarget();

    /**
     * Hidden copy assignment operator.
     */
    RenderTarget& operator=(const RenderTarget&);

    std::string _id;
    Texture* _texture;
};

}

#endif
