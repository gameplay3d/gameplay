#ifndef MESHSKIN_H_
#define MESHSKIN_H_

#include "Matrix.h"

namespace gameplay
{

class Package;
class Model;
class Joint;

/**
 * Represents the skin for a mesh.
 */
class MeshSkin
{
    friend class Package;
    friend class Model;
    friend class Joint;

public:

    /**
     * Returns the bind shape matrix.
     * 
     * @return The bind shape matrix.
     */
    const Matrix& getBindShape() const;

    /**
     * Sets the bind shape of this skin.
     * 
     * @param matrix An array of 16 floats.
     */
    void setBindShape(const float* matrix);

    /**
     * Returns the joint with the given ID.
     * 
     * @param id The ID of the joint to search for.
     * 
     * @return The joint, or NULL if not found.
     */
    Joint* getJoint(const char* id) const;

    /**
     * Returns the pointer to the Vector4 array for the purpose of binding to a shader.
     * 
     * @return The pointer to the matrix palette.
     */
    Vector4* getMatrixPalette() const;

    /**
     * Returns the number of elements in the matrix palette array.
     * Each element is a Vector4* that represents a row.
     * Each matrix palette is represented by 3 rows of Vector4.
     * 
     * @return The matrix palette size.
     */
    unsigned int getMatrixPaletteSize() const;

private:

    /**
     * Constructor.
     */
    MeshSkin();

    /**
     * Destructor.
     */
    ~MeshSkin();

    /**
     * Sets the number of joints that can be stored in this skin.
     * This method allocates the necessary memory.
     * 
     * @param jointCount The new size of the joint vector.
     */
    void setJointCount(unsigned int jointCount);

    /**
     * Sets the joint at the given index and increments the ref count.
     * 
     * @param joint The joint to be set.
     * @param index The index in the joints vector.
     */
    void setJoint(Joint* joint, unsigned int index);

    /**
     * Clears the list of joints and releases each joint.
     */
    void clearJoints();

    /**
     * Returns the joint at the given index.
     * 
     * @param index The index.
     * 
     * @return The joint.
     */
    Joint* getJoint(unsigned int index) const;

    Matrix _bindShape;
    std::vector<Joint*> _joints;

    // Pointer to the array of palette matrices.
    // This array is passed to the vertex shader as a uniform.
    // Each 4x3 row-wise matrix is represented as 3 Vector4's.
    // The number of Vector4's is (_joints.size() * 3).
    Vector4* _matrixPalette;
    Model* _model;
};

}

#endif
