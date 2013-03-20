#include "Base.h"
#include "Sprite.h"
#include "Node.h"
#include "Scene.h"

namespace gameplay
{

static Vector2 _defaultOffset = Vector2::zero();

Sprite::Sprite(const char* id)
	: _node(NULL), 
	_tileSheet(NULL), _tint(Vector4::one()),
	_flip(FLIP_NONE), _defaultTile(), 
	_width(0), _height(0), _x(_defaultOffset.x), _y(_defaultOffset.y), 
	_frame(), _defaultTileInUse(true)
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

const Vector2& Sprite::getDefaultSpriteOffset()
{
	return _defaultOffset;
}

void Sprite::setDefaultSpriteOffset(const Vector2& offset)
{
	_defaultOffset = offset;
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

Vector2 Sprite::getSize() const
{
	return Vector2(_width, _height);
}

float Sprite::getWidth() const
{
	return _width;
}

float Sprite::getHeight() const
{
	return _height;
}

void Sprite::setSize(const Vector2& size)
{
	setSize(size.x, size.y);
}

void Sprite::setSize(float width, float height)
{
	_width = width;
	_height = height;
}

Vector2 Sprite::getOffset() const
{
	return Vector2(_x, _y);
}

float Sprite::getOffsetX() const
{
	return _x;
}

float Sprite::getOffsetY() const
{
	return _y;
}

void Sprite::setOffset(const Vector2& offset)
{
	setOffset(offset.x, offset.y);
}

void Sprite::setOffset(float x, float y)
{
	setOffsetX(x);
	setOffsetY(y);
}

void Sprite::setOffsetX(float value)
{
	_x = value;
}

void Sprite::setOffsetY(float value)
{
	_y = value;
}

const Vector4& Sprite::getTint() const
{
	return _tint;
}

void Sprite::setTint(const Vector4& tint)
{
	_tint = tint;
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
	if (Node* node = getNode())
	{
		//Position
		pos = node->getTranslationWorld();

		//Rotation
		const Quaternion& rot = node->getRotation();
		angle = atan2f(2.0f * rot.x * rot.y + 2.0f * rot.z * rot.w, 1.0f - 2.0f * ((rot.y * rot.y) + (rot.z * rot.z)));

		//Scale the size
		size.x *= node->getScaleX();
		size.y *= node->getScaleY();

		if (isolateDraw)
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

	//Handle flip
	if ((_flip & FLIP_HORZ) == FLIP_HORZ)
	{
		pos.x -= size.x;
		size.x = -size.x;
	}
	if ((_flip & FLIP_VERT) == FLIP_VERT)
	{
		pos.y -= size.y;
		size.y = -size.y;
	}

	//Draw
	if (isolateDraw)
	{
		batch->start();
	}

	//The actual draw call
	batch->draw(pos, _frame, size, _tint, rotationPoint, angle);

	if (isolateDraw)
	{
		batch->finish();
	}
}

unsigned int Sprite::getAnimationPropertyComponentCount(int propertyId) const
{
	switch (propertyId)
	{
		case ANIMATE_SIZE:
		case ANIMATE_OFFSET:
			return 2;
		case ANIMATE_FRAME_INDEX:
			return 3;
		case ANIMATE_TINT:
			return 4;
		case ANIMATE_FRAME_SPECIFIC:
			return 5;
		case ANIMATE_FRAME_BLENDING: //TODO
		case ANIMATE_FRAME_TRANSITIONS: //TODO
		default:
			return -1;
	}
}

void Sprite::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
	GP_ASSERT(value);

    switch (propertyId)
	{
		case ANIMATE_SIZE:
			value->setFloat(0, _width);
			value->setFloat(1, _height);
			break;
		case ANIMATE_OFFSET:
			value->setFloat(0, _x);
			value->setFloat(1, _y);
			break;
		case ANIMATE_FRAME_INDEX:
			//TODO
			break;
		case ANIMATE_FRAME_SPECIFIC:
			//TODO
			break;
		case ANIMATE_FRAME_BLENDING:
			//TODO
			break;
		case ANIMATE_FRAME_TRANSITIONS:
			//TODO
			break;
		case ANIMATE_TINT:
			value->setFloats(0, &_tint.x, 4);
			break;
		default:
			break;
	}
}

void Sprite::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
	GP_ASSERT(value);
    GP_ASSERT(blendWeight >= 0.0f && blendWeight <= 1.0f);

    switch (propertyId)
	{
		case ANIMATE_SIZE:
		{
			setSize(Curve::lerp(blendWeight, _width, value->getFloat(0)), Curve::lerp(blendWeight, _height, value->getFloat(1)));
			break;
		}
		case ANIMATE_OFFSET:
		{
			setOffset(Curve::lerp(blendWeight, _x, value->getFloat(0)), Curve::lerp(blendWeight, _y, value->getFloat(1)));
			break;
		}
		case ANIMATE_FRAME_INDEX:
		{
			//TODO
			break;
		}
		case ANIMATE_FRAME_SPECIFIC:
		{
			//TODO
			break;
		}
		case ANIMATE_FRAME_BLENDING:
		{
			//TODO
			break;
		}
		case ANIMATE_FRAME_TRANSITIONS:
		{
			//TODO
			break;
		}
		case ANIMATE_TINT:
        {
			setTint(Vector4(Curve::lerp(blendWeight, _tint.x, value->getFloat(0)), Curve::lerp(blendWeight, _tint.y, value->getFloat(1)), Curve::lerp(blendWeight, _tint.z, value->getFloat(2)), Curve::lerp(blendWeight, _tint.w, value->getFloat(3))));
            break;
        }
		default:
			break;
	}
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