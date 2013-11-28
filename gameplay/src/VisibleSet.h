#ifndef VISIBLESET_H_
#define VISIBLESET_H_

namespace gameplay
{

class Scene;

/**
 * Represents a set of nodes that are visible from the
 * active camera. This provides an enumerator
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
     * Gets the active camera being used to test the visibility.
     *
     * @return The active camera being used to test the visibility.
     */
    virtual Camera* getActiveCamera() = 0;

    /**
     * Resets the enumerator for enumerating the visible set.
     */
    virtual void reset() = 0;

    /**
     * Gets the next visible node in the set or NULL
     * once you have reached the end of the set.
     *
     * @return The next visible node in the set or NULL at end of set.
     */
    virtual Node* getNext() = 0;
};

}

#endif
