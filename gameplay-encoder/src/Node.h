#ifndef NODE_H_
#define NODE_H_

#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"

namespace gameplay
{

class Node : public Object
{
public:

    /**
     * Constructor.
     */
    Node(void);

    /**
     * Destructor.
     */
    virtual ~Node(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    /**
     * Adds a child node.
     *
     * If the ownChild is true. The child is deleted when this
     * node is deleted.
     *
     * @param child The child to add.
     */
    void addChild(Node* child);

    /**
     * Removes a child node.
     *
     * If no longer referenced it destroys the node.
     * To avoid destroy on changing hierarchy, ensure you add first before removing.
     *
     * @param child The child to remove.
     */
    void removeChild(Node* child);

    /**
     * Removes all the child node.
     */
    void removeChildren();

    /**
     * Determines if this node has child nodes.
     *
     * @return true if it has 1 or more children; false if otherwise.
     */
    bool hasChildren() const;

    /**
     * Get the number of children for this node.
     *
     * @return The number of child nodes for this node.
     */
    unsigned int getChildCount() const;

    /**
     * Gets the next sibling node.
     *
     * @return The next sibling node.
     */
    Node* getNextSibling() const;

    /**
     * Gets the previous sibling node.
     *
     * @return The previous sibling node.
     */
    Node* getPreviousSibling() const;

    /**
     * Gets the first child node.
     *
     * @return The first child node.
     */
    Node* getFirstChild() const;

    /**
     * Gets the last child node.
     *
     * @return The last child node.
     */
    Node* getLastChild() const;

    /**
     * Gets the parent node.
     *
     * @return The parent node.
     */
    Node* getParent() const;

    /**
     * Returns the Camera for this node.
     * 
     * @return The camera for this node or NULL if no camera is set.
     */
    Camera* getCamera() const;

    /**
     * Returns the Light for this node.
     * 
     * @return The light for this node or NULL if no light is set.
     */
    Light* getLight() const;

    /**
     * Returns the Model of this node.
     * 
     * @return The model for this node or NULL if no model is set.
     */
    Model* getModel() const;

    /**
     * Returns the transform matrix for the node.
     */
    const Matrix& getTransformMatrix() const;

    /**
     * Sets the transform for this node.
     */
    void setTransformMatrix(float matrix[]);

    /**
     * Returns the resolved world matrix for the node.
     */
    const Matrix& getWorldMatrix() const;

    /*
     * Resets the node's transform matrix to the identity matrix.
     */
    void resetTransformMatrix();

    void setCamera(Camera* camera);
    void setLight(Light* light);
    void setModel(Model* model);

    /**
     * Sets if this node is a joint node.
     */
    void setIsJoint(bool value);

    /**
     * Returns true if this is a joint node.
     */
    bool isJoint() const;

    Node* getFirstCameraNode() const;

    /**
     * Returns true if this node has a camera.
     */
    bool hasCamera() const;

    /**
     * Returns true if this node has a light.
     */
    bool hasLight() const;
    
private:

    Matrix _transform;
    mutable Matrix _worldTransform;

    int _childCount;
    Node* _nextSibling;
    Node* _previousSibling;
    Node* _firstChild;
    Node* _lastChild;
    Node* _parent;

    Camera* _camera;
    Light* _light;
    Model* _model;

    bool _joint;
};

}

#endif
