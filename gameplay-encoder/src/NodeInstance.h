#ifndef NODEINSTANCE_H_
#define NODEINSTANCE_H_

#include "Object.h"

namespace gameplay
{

class Node;

class NodeInstance : public Object
{
public:
    NodeInstance(void);
    virtual ~NodeInstance(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    void setNode(Node* node);
private:
    Node* ref;
};

}
#endif
