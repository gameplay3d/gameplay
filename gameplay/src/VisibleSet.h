#ifndef VISIBLESET_H_
#define VISIBLESET_H_

#include "Scene.h"

namespace gameplay
{

/**
 * Represents a set of nodes that are visible from the
 * scenes active camera. This provides an enumerator
 * to traverse the scene returning only visible nodes.
 */
class VisibleSet
{
public:
    
    /**
     * Destructor.
     */
    virtual ~VisibleSet() { }

    /**
     * Sets the scene to determine the visible set from.
     *
     * @param scene The scene to determine the visible set from.
     */
    virtual void setScene(Scene* scene) = 0;

    /**
     * Gets the scene to determine the visible set from.
     *
     * @return The scene to determine the visible set from.
     */
    virtual Scene* getScene() = 0;

    /**
     * Resets the enumerator for enumerating the visible set.
     */
    virtual void reset() = 0;

    /**
     * Gets the next visible node in the set or NULL 
     * onces you have reached the end of the set.
     * 
     * @return The next visible node in the set or NULL at end of set.
     */
    virtual Node* getNext() = 0;
};

}

#endif