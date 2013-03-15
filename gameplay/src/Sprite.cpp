#include "Base.h"
#include "Sprite.h"
#include "Node.h"

//TODO: Need to create Lua binding

namespace gameplay
{

Sprite::Sprite(const char* id)
	: _node(NULL), _tileSheet(NULL)
{
	if (id)
    {
        _id = id;
    }
}

Sprite::~Sprite()
{
	SAFE_RELEASE(_tileSheet);
}

Sprite* Sprite::create(const char* id, TileSheet* tileSheet)
{
	GP_ASSERT(tileSheet);
	tileSheet->addRef();

	Sprite* sprite = new Sprite(id);
	sprite->_tileSheet = tileSheet;
	return sprite;
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

TileSheet* Sprite::getTileSheet()
{
	return _tileSheet;
}

unsigned int Sprite::getAnimationPropertyComponentCount(int propertyId) const
{
	//TODO
	return 0;
}

void Sprite::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
	//TODO
}

void Sprite::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
	//TODO
}

Sprite* Sprite::clone(NodeCloneContext &context)
{
	Sprite* copy = create(getId(), getTileSheet());
	cloneInto(copy, context);
	return copy;
}

void Sprite::cloneInto(Sprite* sprite, NodeCloneContext &context) const
{
	//TODO
}

}