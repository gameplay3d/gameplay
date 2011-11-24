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

class MeshSkin : public Object
{
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

    const std::list<std::string>& getJointNames();

    void setBindShape(const float data[]);

    void setVertexInfluenceCount(unsigned int count);

    void setJointNames(const std::list<std::string>& list);

    void setJoints(const std::list<Node*>& list);

    void setBindPoses(std::vector<Matrix>& list);

    /**
     * Returns true if the MeshSkin contains a joint with the given ID.
     * 
     * @param id The ID of the joint to search for.
     * 
     * @return True if the joint belongs to this skin, false otherwise.
     */
    bool hasJoint(const char* id);

private:

    float _bindShape[16];
    std::list<Node*> _joints;
    std::list<float> _bindPoses;

    std::list<std::string> _jointNames;

    unsigned int _vertexInfluenceCount;
};

}
#endif
