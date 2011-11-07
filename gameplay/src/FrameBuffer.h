#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "Base.h"
#include "Ref.h"

namespace gameplay
{

class RenderTarget;
class DepthStencilTarget;

class FrameBuffer : public Ref
{
public:

    /**
     * Create an empty FrameBuffer and add it to the list of available FrameBuffers.
     *
     * @param id The ID of the new FrameBuffer.  Uniqueness is recommended but not enforced.
     *
     * @return A newly created FrameBuffer.
     */
    static FrameBuffer* create(const char* id);

    /**
     * Create an empty FrameBuffer with a RenderTarget of the specified width and height,
     * and add the FrameBuffer to the list of available FrameBuffers.
     *
     * @param id The ID of the new FrameBuffer.  Uniqueness is recommended but not enforced.
     * @param width The width of the RenderTarget to be created and attached.
     * @param height The height of the RenderTarget to be created and attached.
     *
     * @return A newly created FrameBuffer.
     */
    static FrameBuffer* create(const char* id, unsigned int width, unsigned int height);

    /**
     * Get a named FrameBuffer from its ID.
     *
     * @param id The ID of the FrameBuffer to search for.
     *
     * @return The FrameBuffer with the specified ID, or NULL if one was not found.
     */
    static FrameBuffer* getFrameBuffer(const char* id);
     
    /**
     * Destructor.  Removes this FrameBuffer from the list of available FrameBuffers.
     */
    ~FrameBuffer();

    /**
     * Get the ID of this FrameBuffer.
     *
     * @return The ID of this FrameBuffer.
     */
    const char* getID() const;

    /**
     * Get the number of color attachments available on the current hardware.
     *
     * @return The number of color attachments available on the current hardware.
     */
    static unsigned int getMaxRenderTargets();
 
    /**
     * Set a RenderTarget on this FrameBuffer's color attachment at the specified index.
     *
     * @param target The RenderTarget to set.
     * @param index The index of the color attachment to set.
     */
    void setRenderTarget(RenderTarget* target, unsigned int index = 0);
 
    /**
     * Get the RenderTarget attached to the FrameBuffer's color attachment at the specified index.
     *
     * @param index The index of the color attachment to retrieve a RenderTarget from.
     *
     * @return The RenderTarget attached at the specified index.
     */
    RenderTarget* getRenderTarget(unsigned int index = 0) const;
 
    /**
     * Set this FrameBuffer's DepthStencilTarget.
     *
     * @param target The DepthStencilTarget to set on this FrameBuffer.
     */
    void setDepthStencilTarget(DepthStencilTarget* target);
  
    /**
     * Get this FrameBuffer's DepthStencilTarget.
     *
     * @return This FrameBuffer's DepthStencilTarget.
     */
    DepthStencilTarget* getDepthStencilTarget() const;
 
    /**
     * Binds this FrameBuffer for off-screen rendering.
     */
    void bind();

    /**
     * Binds the default FrameBuffer for rendering to the display.
     */
    static void bindDefault(); 
     
private:
 
    FrameBuffer(const char* id);

    std::string _id;
    FrameBufferHandle _handle;
    RenderTarget** __renderTargets;
    DepthStencilTarget* _depthStencilTarget;
    unsigned int _index;
};

}

#endif