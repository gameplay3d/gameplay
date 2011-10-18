#include "NodeInstance.h"
#include "Node.h"

namespace gameplay
{

NodeInstance::NodeInstance(void) : ref(NULL)
{
}

NodeInstance::~NodeInstance(void)
{
}

unsigned int NodeInstance::getTypeId(void) const
{
    return NODEINSTANCE_ID;
}

const char* NodeInstance::getElementName(void) const
{
    return "NodeInstance";
}

void NodeInstance::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    if (ref != NULL)
        ref->writeBinaryXref(file);
    else
        write((unsigned int)0, file);
}

void NodeInstance::writeText(FILE* file)
{
    fprintElementStart(file);
    if (ref != NULL)
    {
        fprintfElement(file, "ref", ref->getId());
    }
    fprintElementEnd(file);
}

void NodeInstance::setNode(Node* node)
{
    ref = node;
}

}
