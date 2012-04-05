#ifndef JOINT_H_
#define JOINT_H_

#include "Node.h"

namespace gameplay
{

class MeshSkin;
class Package;

/**
 * Defines a basic hierachial structure of transformation spaces.
 */
class Joint : public Node
{
    friend class Node;
    friend class MeshSkin;
    friend class Package;

public:

    /**
     * @see Node::getType()
     */
    Node::Type getType() const;

    /**
     * Returns the inverse bind pose matrix for this joint.
     * 
     * @return Inverse bind pose matrix.
     */
    const Matrix& getInverseBindPose() const;

protected:

    /**
     * Constructor.
     */
    Joint(const char* id);

    /**
     * Destructor.
     */
    virtual ~Joint();

    /**
     * Creates a new joint with the given id.
     * 
     * @param id ID string.
     * 
     * @return Newly created joint.
     */
    static Joint* create(const char* id);

    /**
     * Clones a single node and its data but not its children.
     * This method returns a node pointer but actually creates a Joint.
     * 
     * @param context The clone context.
     * 
     * @return Pointer to the newly created joint.
     */
    virtual Node* cloneSingleNode(NodeCloneContext &context) const;

    /**
     * Sets the inverse bind pose matrix.
     * 
     * @param m Matrix representing the inverse bind pose for this Joint.
     */
    void setInverseBindPose(const Matrix& m);

    void updateJointMatrix(const Matrix& bindShape, Vector4* matrixPalette);

    void transformChanged();

private:

    /**
     * Constructor.
     */
    Joint(const Joint& copy);

    /**
     * Copy assignment operator.
     */
    Joint& operator=(const Joint&);

protected:

    Matrix _bindPose;
    bool _jointMatrixDirty;
    unsigned int _skinCount;
};

}

#endif
