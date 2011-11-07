#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_

#include "Base.h"
#include "Ref.h"

namespace gameplay
{

class Texture;

class RenderTarget : public Ref
{
public:
 
    /**
     * Create a RenderTarget and add it to the list of available RenderTargets.
     *
     * @param id The ID of the new RenderTarget.  Uniqueness is recommended but not enforced.
     * @param width The width of the new RenderTarget.
     * @param height The height of the new RenderTarget.
     *
     * @return A newly created RenderTarget.
     */
    static RenderTarget* create(const char* id, unsigned int width, unsigned int height);

    /**
     * Get a named RenderTarget from its ID.
     *
     * @param id The ID of the RenderTarget to search for.
     *
     * @return The RenderTarget with the specified ID, or NULL if one was not found.
     */
    static RenderTarget* getRenderTarget(const char* id);
 
    /**
     * Destructor.  Removes this RenderTarget from the list of available RenderTargets.
     */
    ~RenderTarget();

    /**
     * Get the ID of this RenderTarget.
     *
     * @return The ID of this RenderTarget.
     */
    const char* getID() const;

    /**
     * Get the backing texture of this RenderTarget.
     *
     * @return The backing texture of this RenderTarget.
     */
    Texture* getTexture() const;
 
private:
 
    RenderTarget(const char* id, unsigned int width, unsigned int height);
    RenderTarget();

    std::string _id;
    Texture* _texture;
    unsigned int _index; // Index into vector.
};

}

#endif