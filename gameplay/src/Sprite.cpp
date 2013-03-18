#include "Base.h"
#include "Sprite.h"
#include "Node.h"
#include "Scene.h"

namespace gameplay
{

Sprite::Sprite(const char* id)
	: _node(NULL), _tileSheet(NULL), _flip(FLIP_NONE), _defaultTile(), _width(0), _height(0), _x(0), _y(0), _frame(), _defaultTileInUse(true)
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

	sprite->_frame = sprite->_defaultTile;

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
	if(_defaultTileInUse)
	{
		_frame = tile;
	}
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

const Rectangle& Sprite::getCurrentAnimationFrame() const
{
	return _frame;
}

void Sprite::draw(bool isolateDraw)
{
	SpriteBatch* batch = _tileSheet->getSpriteBatch();

	static Vector2 rotationPoint = Vector2::one() * 0.5f;

	Vector3 pos;
	Vector2 size = Vector2(_width, _height);
	float angle = 0.0f;

	//Adjust values that rely on Node
	if(Node* node = getNode())
	{
		pos = node->getTranslationWorld();

		angle = (2.0f * acos(node->getRotation().w)); //XXX Rotation is still iffy

		//Scale the size
		size.x *= node->getScaleX();
		size.y *= node->getScaleY();

		if(isolateDraw)
		{
			//Adjust projection matrix if this draw is specific to this Sprite
			if(Camera* activeCamera = node->getScene()->getActiveCamera())
			{
				batch->setProjectionMatrix(activeCamera->getViewProjectionMatrix());
			}
		}
	}
	else
	{
		pos = Vector3::zero();
	}

	//Offset position
	pos.x += _x;
	pos.y += _y;

	if(isolateDraw)
	{
		batch->start();
	}

	//The actual draw call
	batch->draw(pos, _frame, size, Vector4::one(), rotationPoint, angle);

	if(isolateDraw)
	{
		batch->finish();
	}
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
	sprite->_defaultTile = _defaultTile;
	sprite->_width = _width;
	sprite->_height = _height;
	sprite->_x = _x;
	sprite->_y = _y;
	sprite->_frame = _frame;
	sprite->_defaultTileInUse = _defaultTileInUse;
}

}