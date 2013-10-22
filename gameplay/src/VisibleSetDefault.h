#ifndef VISIBLESETDEFAULT_H_
#define VISIBLESETDEFAULT_H_

#include "VisibleSet.h"

namespace gameplay
{

/**
 * Represents a default implementation of a VisibleSet.
 *
 * All nodes visible to the active camera in the scene are returned.
 * @see VisibleSet
 */
class VisibleSetDefault : public VisibleSet
{
public:

    /**
     * Creates a new default VisibleSet
     *
     * @param scene The scene to use for determining visible nodes.
     */
    static VisibleSetDefault* create(Scene* scene);

    /**
     * @see VisibleSet::setScene
     */
    void setScene(Scene* scene);

    /**
     * @see VisibleSet::getScene
     */
    Scene* getScene();

    /**
     * @see VisibleSet::reset
     */
    void reset();

    /**
     * @see VisibleSet::getNext
     */
    Node* getNext();

private:
    
    /**
     * Constructor.
     */
    VisibleSetDefault(Scene* scene);

    /**
     * Constructor.
     */
    VisibleSetDefault(const VisibleSetDefault& copy);

    /**
     * Destructor.
     */
    ~VisibleSetDefault();

    bool isNodeVisible(Node* node);

private:

    Scene* _scene;
    Node* _next;
};

}

#endif