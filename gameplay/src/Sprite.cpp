#include "Base.h"
#include "Sprite.h"
#include "Node.h"

//TODO: Need to create Lua binding
//TODO: Need to update Lua bindings for Node

namespace gameplay
{

Sprite::Sprite(const char* id)
	: _node(NULL)
{
	if (id)
    {
        _id = id;
    }
}

Sprite::~Sprite()
{
}

const char* Sprite::getId() const
{
    return _id.c_str();
}

Node* Sprite::getNode() const
{
	return _node;
}

void Sprite::setNode(Node* node)
{
	_node = node;
}

}