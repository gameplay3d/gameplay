#include "Base.h"
#include "Sprite.h"
#include "Node.h"
#include "Scene.h"

namespace gameplay
{

static Vector2 _defaultOffset = Vector2::zero();

Sprite::Sprite(const char* id)
	: _stripIndex(0), _stripFrame(0), _defaultTileInUse(true), _frame(),
	_node(NULL),
	_tileSheet(NULL), _tint(Vector4::one()),
	_flip(FLIP_NONE), _defaultTile(), 
	_width(0), _height(0), _x(_defaultOffset.x), _y(_defaultOffset.y)
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
		if (rot.x != 0.0f || rot.y != 0.0f || rot.z != 0.0f) //We only want to check these values to determine if we need to calculate rotation.
		{
			angle = atan2f(2.0f * rot.x * rot.y + 2.0f * rot.z * rot.w, 1.0f - 2.0f * ((rot.y * rot.y) + (rot.z * rot.z)));
		}

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
		pos.x += size.x;
		size.x = -size.x;
	}
	if ((_flip & FLIP_VERT) == FLIP_VERT)
	{
		pos.y += size.y;
		size.y = -size.y;
	}

	//Draw
	if (isolateDraw)
	{
		batch->start();
	}

	//The actual draw call
	batch->draw(pos, _defaultTileInUse ? _defaultTile : _frame, size, _tint, rotationPoint, angle);

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
		case ANIMATE_FRAME_INDEX:
			return 2;
		case ANIMATE_TINT:
		case ANIMATE_FRAME_SPECIFIC:
			return 4;
		default:
			return -1;
	}
}

int Sprite::getPropertyId(const char* propertyIdStr)
{
	GP_ASSERT(propertyIdStr);

	if (_targetType == AnimationTarget::TRANSFORM)
    {
        if (strcmp(propertyIdStr, "ANIMATE_SIZE") == 0)
        {
            return Sprite::ANIMATE_SIZE;
        }
		else if (strcmp(propertyIdStr, "ANIMATE_OFFSET") == 0)
        {
            return Sprite::ANIMATE_OFFSET;
        }
		else if (strcmp(propertyIdStr, "ANIMATE_FRAME_INDEX") == 0)
        {
            return Sprite::ANIMATE_FRAME_INDEX;
        }
		else if (strcmp(propertyIdStr, "ANIMATE_FRAME_SPECIFIC") == 0)
        {
            return Sprite::ANIMATE_FRAME_SPECIFIC;
        }
		else if (strcmp(propertyIdStr, "ANIMATE_TINT") == 0)
        {
            return Sprite::ANIMATE_TINT;
        }
	}

	return AnimationTarget::getPropertyId(propertyIdStr);
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
			value->setFloat(0, (float)_stripIndex);
			value->setFloat(1, (float)_stripFrame);
			break;
		case ANIMATE_FRAME_SPECIFIC:
			value->setFloats(0, &_frame.x, 4);
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
			_defaultTileInUse = true; //Assume it didn't work

			//Get the values
			float indices[2];
			value->getFloats(0, indices, 2);

			//Make sure the values are within range
			if(indices[0] >= 0 && indices[1] >= 0)
			{
				//Get the strip
				unsigned int index = (unsigned int)floorf(indices[0]);
				if(index < _tileSheet->_strips.size())
				{
					TileSheet::FrameStrip& strip = _tileSheet->_strips[_stripIndex = index];

					//Get the frame
					index = (unsigned int)floorf(indices[1]);
					if(index < strip._frameCount)
					{
						Rectangle& frame = strip._frames[_stripFrame = index];

						_defaultTileInUse = false; //We have a valid frame

						//Setup frame
						_frame.x = Curve::lerp(blendWeight, _frame.x, frame.x);
						_frame.y = Curve::lerp(blendWeight, _frame.y, frame.y);
						_frame.width = Curve::lerp(blendWeight, _frame.width, frame.width);
						_frame.height = Curve::lerp(blendWeight, _frame.height, frame.height);
					}
				}
			}
			break;
		}
		case ANIMATE_FRAME_SPECIFIC:
		{
			_defaultTileInUse = false; //Indicate to use it since we are handling an animation
			_frame.x = Curve::lerp(blendWeight, _frame.x, value->getFloat(0));
			_frame.y = Curve::lerp(blendWeight, _frame.y, value->getFloat(1));
			_frame.width = Curve::lerp(blendWeight, _frame.width, value->getFloat(2));
			_frame.height = Curve::lerp(blendWeight, _frame.height, value->getFloat(3));
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
	GP_ASSERT(sprite);

	//Clone animation
	AnimationTarget::cloneInto(static_cast<AnimationTarget*>(sprite), context);

	//Get copied node if it exists
	if (Node* node = getNode())
    {
        Node* clonedNode = context.findClonedNode(node);
        if (clonedNode)
        {
            sprite->setNode(clonedNode);
        }
    }

	//Clone settings
	sprite->_flip = _flip;
	sprite->_defaultTile = _defaultTile;
	sprite->_width = _width;
	sprite->_height = _height;
	sprite->_x = _x;
	sprite->_y = _y;
	sprite->_tint = _tint;

	//Clone animation info
	sprite->_frame = _frame;
	sprite->_defaultTileInUse = _defaultTileInUse;
	sprite->_stripIndex = _stripIndex;
	sprite->_stripFrame = _stripFrame;
}

}