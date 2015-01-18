#include "Base.h"
#include "Drawable.h"
#include "Node.h"


namespace gameplay
{

Drawable::Drawable(Type type)
    : _node(NULL), _type(type)
{
}

Drawable::~Drawable()
{
}

Node* Drawable::getNode() const
{
    return _node;
}

void Drawable::setNode(Node* node)
{
    _node = node;
}

Drawable::Type Drawable::getType() const
{
    return _type;
}

}
