#ifndef JOINT_H_
#define JOINT_H_

#include "Node.h"

namespace gameplay
{

class MeshSkin;
class Bundle;

/**
 * Defines a basic hierachial structure of transformation spaces.
 */
class Joint : public Node
{
    friend class Node;
    friend class MeshSkin;
    friend class Bundle;

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

    /**
     * Updates the joint matrix.
     * 
     * @param bindShape The bind shape matrix.
     * @param matrixPalette The matrix palette to update.
     */
    void updateJointMatrix(const Matrix& bindShape, Vector4* matrixPalette);

    /**
     * Called when this Joint's transform changes.
     */
    void transformChanged();

private:

    /**
     * Constructor.
     */
    Joint(const Joint& copy);

    /**
     * Hidden copy assignment operator.
     */
    Joint& operator=(const Joint&);

protected:

    /** 
     * The Matrix representation of the Joint's bind pose.
     */
    Matrix _bindPose;
    
    /** 
     * Flag used to mark if the Joint's matrix is dirty.
     */
    bool _jointMatrixDirty;
    
    /** 
     * The number of MeshSkin's influencing the Joint.
     */
    unsigned int _skinCount;
};

}

#endif
