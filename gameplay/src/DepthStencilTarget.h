#ifndef DEPTHSTENCILTARGET_H_
#define DEPTHSTENCILTARGET_H_

#include "Base.h"
#include "Ref.h"

namespace gameplay
{

class DepthStencilTarget : public Ref
{
public:
 
    /**
     * Create a DepthStencilTarget and add it to the list of available DepthStencilTargets.
     *
     * @param id The ID of the new DepthStencilTarget.  Uniqueness is recommended but not enforced.
     *
     * @return A newly created DepthStencilTarget.
     */
    static DepthStencilTarget* create(const char* id, unsigned int width, unsigned int height);

    /**
     * Get a named DepthStencilTarget from its ID.
     *
     * @param id The ID of the DepthStencilTarget to search for.
     *
     * @return The DepthStencilTarget with the specified ID, or NULL if one was not found.
     */
    static DepthStencilTarget* getDepthStencilTarget(const char* id);

    /**
     * Destructor.  Removes this DepthStencilTarget from the list of available DepthStencilTargets.
     */
    ~DepthStencilTarget();

    /**
     * Get the ID of this DepthStencilTarget.
     *
     * @return The ID of this DepthStencilTarget.
     */
    const char* getID() const;
 
private:
 
    DepthStencilTarget(const char* id, unsigned int width, unsigned int height);

    std::string _id;
    RenderBufferHandle _handle;
    unsigned int _index;
};

}

#endif