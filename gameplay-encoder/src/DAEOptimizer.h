#ifndef DAEOPTIMIZER_H_
#define DAEOPTIMIZER_H_

#include "DAEUtil.h"

namespace gameplay
{

/**
 * The DAEOptimizer optimizes a COLLADA dom.
 */
class DAEOptimizer
{
public:

    /**
     * Constructor.
     */
    DAEOptimizer(domCOLLADA* dom);

    /**
     * Destructor.
     */
    ~DAEOptimizer(void);

    /**
     * Combines all of the animations that target the node and all of its child nodes into a new animation with the given ID.
     * 
     * @param nodeId The ID of the node.
     * @param animationId The ID of the new animation to create.
     */
    void combineAnimations(const std::string& nodeId, const std::string& animationId);

private:

    /**
     * Gets all of the animation channels that target the given node and appends them to the list.
     * 
     * @param node The node that the animation channels target.
     * @param channels The list of channels to append to.
     */
    void getAnimationChannels(const domNodeRef& node, std::list<domChannelRef>& channels);

    /**
     * Deletes all of the empty animations in the dom.
     */
    void deleteEmptyAnimations();

private:
    
    domCOLLADA* _dom;
    std::string _inputPath;
};

}

#endif
