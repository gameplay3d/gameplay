#include "Base.h"
#include "Sprite.h"
#include "Node.h"

namespace gameplay
{

Sprite::Sprite(const char* id)
	: _node(NULL), _tileSheet(NULL), _flip(FLIP_NONE)
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

int Sprite::getFlip() const
{
	return _flip;
}

void Sprite::setFlip(int flip)
{
	_flip = flip & (FLIP_HORZ | FLIP_VERT);
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

void Sprite::draw()
{
	//TODO
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