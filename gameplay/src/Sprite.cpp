#include "Base.h"
#include "Sprite.h"
#include "Node.h"

namespace gameplay
{

Sprite::Sprite(const char* id)
	: _node(NULL), _tileSheet(NULL), _flip(FLIP_NONE), _defaultTile(), _width(0), _height(0), _x(0), _y(0)
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
	
	Texture* tex = tileSheet->getSpriteBatch()->getSampler()->getTexture();
	sprite->_defaultTile.width = sprite->_width = tex->getWidth();
	sprite->_defaultTile.height = sprite->_height = tex->getHeight();

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

const Rectangle& Sprite::getDefaultTile() const
{
	return _defaultTile;
}

void Sprite::setDefaultTile(const Rectangle& tile)
{
	_defaultTile = tile;
}

Vector2 Sprite::getSpriteSize() const
{
	return Vector2(_width, _height);
}

float Sprite::getSpriteWidth() const
{
	return _width;
}

float Sprite::getSpriteHeight() const
{
	return _height;
}

void Sprite::setSpriteSize(const Vector2& size)
{
	setSpriteSize(size.x, size.y);
}

void Sprite::setSpriteSize(float width, float height)
{
	_width = width;
	_height = height;
}

Vector2 Sprite::getSpriteOffset() const
{
	return Vector2(_x, _y);
}

float Sprite::getSpriteOffsetX() const
{
	return _x;
}

float Sprite::getSpriteOffsetY() const
{
	return _y;
}

void Sprite::setSpriteOffset(const Vector2& offset)
{
	setSpriteOffset(offset.x, offset.y);
}

void Sprite::setSpriteOffset(float x, float y)
{
	setSpriteOffsetX(x);
	setSpriteOffsetY(y);
}

void Sprite::setSpriteOffsetX(float value)
{
	_x = value;
}

void Sprite::setSpriteOffsetY(float value)
{
	_y = value;
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
	SpriteBatch* batch = _tileSheet->getSpriteBatch();
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
	if (Node* node = getNode())
    {
        Node* clonedNode = context.findClonedNode(node);
        if (clonedNode)
        {
            sprite->setNode(clonedNode);
        }
    }

	sprite->_flip = _flip;
	sprite->_width = _width;
	sprite->_height = _height;
	sprite->_x = _x;
	sprite->_y = _y;
}

}