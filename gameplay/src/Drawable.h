#ifndef DRAWABLE_H_
#define DRAWABLE_H_

namespace gameplay
{

class Node;
class NodeCloneContext;

/**
 * Defines a drawable object that can be attached to a Node.
 */
class Drawable
{
    friend class Node;

public:

    /**
     * Constructor.
     */
    Drawable();

    /**
     * Destructor.
     */
    virtual ~Drawable();

    /**
     * Draws the object.
     *
     * @param wireframe true if you want to request to draw the wireframe only.
     * @return The number of graphics draw calls required to draw the object.
     */

    virtual unsigned int draw(bool wireframe = false) = 0;

    /**
     * Gets the node this drawable is attached to.
     *
     * @return The node this drawable is attached to.
     */
    Node* getNode() const;

protected:

    /**
     * Clones the drawable and returns a new drawable.
     *
     * @param context The clone context.
     * @return The newly created drawable.
     */
    virtual Drawable* clone(NodeCloneContext& context) = 0;

    /**
     * Sets the node this drawable is attached to.
     *
     * @param node The node this drawable is attached to.
     */
    virtual void setNode(Node* node);

    /**
     * Node this drawable is attached to.
     */
    Node* _node;
};

}

#endif
