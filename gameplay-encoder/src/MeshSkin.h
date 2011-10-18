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

    void setBindShape(const float data[]);

    void setVertexInfluenceCount(unsigned int count);

    void setNode(Node* node);

    void setJointNames(const std::list<std::string>& list);

    const std::list<std::string>& getJointNames();

    void setJoints(const std::list<Node*>& list);

    void setBindPoses(std::vector<Matrix>& list);

    /**
     * Returns true if the MeshSkin contains a joint with the given ID.
     * 
     * @param id The ID of the joint to search for.
     * @return True if the joint belongs to this skin, false otherwise.
     */
    bool hasJoint(const char* id);

    /**
     * Returns the animation that contains the animation channels that target this skin's joints.
     * 
     * @return The animation for this skin.
     */
    Animation* getAnimation();

    /**
     * Sets the id of the skin's animation.
     * 
     * @param id The new ID.
     */
    void setAnimationId(const char* id);

private:

    Node* influences;
    float bindShape[16];
    std::list<Node*> joints;
    std::list<float> bindPoses;

    std::list<std::string> jointNames;

    unsigned int vertexInfluenceCount;

    Animation* animation;
};

}
#endif
