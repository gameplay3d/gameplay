#ifndef MESHSKIN_H_
#define MESHSKIN_H_

#include <vector>

#include "Base.h"
#include "Object.h"
#include "Matrix.h"
#include "Animation.h"

namespace gameplay
{

class Node;
class Mesh;

struct BoundingSphere
{
    Vector3 center;
    float radius;

    BoundingSphere() : radius(0)
    {
    }

    BoundingSphere(const BoundingSphere& copy)
    {
        set(copy);
    }

    void set(const BoundingSphere& copy)
    {
        center = copy.center;
        radius = copy.radius;
    }
};

class MeshSkin : public Object
{
    friend class Model;

public:

    /**
     * Constructor.
     */
    MeshSkin(void);

    /**
     * Destructor.
     */
    virtual ~MeshSkin(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    void setBindShape(const float data[]);

    void setVertexInfluenceCount(unsigned int count);

    void setJointNames(const std::vector<std::string>& list);

    const std::vector<std::string>& getJointNames();

    void setJoints(const std::vector<Node*>& list);

    void setBindPoses(std::vector<Matrix>& list);

    /**
     * Returns true if the MeshSkin contains a joint with the given ID.
     * 
     * @param id The ID of the joint to search for.
     * 
     * @return True if the joint belongs to this skin, false otherwise.
     */
    bool hasJoint(const char* id);

    void computeBounds();

private:

    Mesh* _mesh;
    float _bindShape[16];
    std::vector<Node*> _joints;
    std::vector<Matrix> _bindPoses;
    std::vector<std::string> _jointNames;
    unsigned int _vertexInfluenceCount;
    std::vector<BoundingSphere> _jointBounds;
};

}

#endif
