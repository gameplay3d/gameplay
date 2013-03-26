#include "Base.h"
#include "SpriteGroup.h"
#include "Node.h"
#include "Scene.h"

namespace gameplay
{

// Global list of active groups
static std::vector<SpriteGroup*> _spriteGroups;

SpriteGroup::SpriteGroup(const char* id)
	: Sprite(id), _groupWidth(1), _groupHeight(1), _children(), _horzGap(0), _vertGap(0)
{
}

SpriteGroup::~SpriteGroup()
{
	//Unreference any Sprites that are used
	for(std::map<unsigned int, Sprite*>::iterator it = _children.begin(); it != _children.end(); it++)
	{
		Sprite* s = it->second;
		if(s)
		{
			SAFE_RELEASE(s);
		}
	}

	// Remove self from vector.
    std::vector<SpriteGroup*>::iterator it = std::find(_spriteGroups.begin(), _spriteGroups.end(), this);
    if (it != _spriteGroups.end())
    {
        _spriteGroups.erase(it);
    }
}

SpriteGroup* SpriteGroup::create(const char* id, unsigned int width, unsigned int height, TileSheet* tileSheet)
{
	GP_ASSERT(width > 0 && height > 0 && tileSheet);
	tileSheet->addRef();

	SpriteGroup* group = new SpriteGroup(id);
	group->_tileSheet = tileSheet;

	group->_groupWidth = width;
	group->_groupHeight = height;

	//Similar to creating from a Sprite, we clone a Sprite to save ourselves the trouble
	Sprite* sp = Sprite::create(NULL, tileSheet);

	NodeCloneContext fake;
	sp->cloneInto(static_cast<Sprite*>(group), fake);

	SAFE_RELEASE(sp);

	//Save the SpriteGroup
	_spriteGroups.push_back(group);

	return group;
}

SpriteGroup* SpriteGroup::createFrom(const char* id, unsigned int width, unsigned int height, Sprite* sprite)
{
	GP_ASSERT(width > 0 && height > 0 && sprite);

	SpriteGroup* group = SpriteGroup::create(id, width, height, sprite->_tileSheet);

	//Clone the Sprite values
	NodeCloneContext fake;
	sprite->cloneInto(static_cast<Sprite*>(group), fake);

	return group;
}

SpriteGroup* SpriteGroup::getSpriteGroup(const char* id)
{
	GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<SpriteGroup*>::const_iterator it;
    for (it = _spriteGroups.begin(); it != _spriteGroups.end(); ++it)
    {
        SpriteGroup* sg = *it;
        GP_ASSERT(sg);
		if (sg->_id == id)
        {
            return sg;
        }
    }
    return NULL;
}

unsigned int SpriteGroup::getGroupWidth() const
{
	return _groupWidth;
}

unsigned int SpriteGroup::getGroupHeight() const
{
	return _groupHeight;
}

float SpriteGroup::getHorzGap() const
{
	return _horzGap;
}

void SpriteGroup::setHorzGap(float gap)
{
	_horzGap = gap;
}

float SpriteGroup::getVertGap() const
{
	return _vertGap;
}

void SpriteGroup::setVertGap(float gap)
{
	_vertGap = gap;
}

int SpriteGroup::getSpriteType(unsigned int x, unsigned int y) const
{
	if(x < _groupWidth && y < _groupHeight)
	{
		unsigned int index = x * _groupWidth + y;
		std::map<unsigned int, Sprite*>::const_iterator it = _children.find(index);
		if(it == _children.end())
		{
			return TYPE_DEFAULT;
		}
		else if(it->second == NULL)
		{
			return TYPE_TRANSPARENT;
		}
		else
		{
			return TYPE_CHILD;
		}
	}
	return -1;
}

bool SpriteGroup::setSpriteType(unsigned int x, unsigned int y, int type)
{
	if(x < _groupWidth && y < _groupHeight && type != TYPE_CHILD && (type == TYPE_DEFAULT || type == TYPE_TRANSPARENT))
	{
		unsigned int index = x * _groupWidth + y;
		std::map<unsigned int, Sprite*>::const_iterator it = _children.find(index);
		if(it == _children.end())
		{
			if(type == TYPE_TRANSPARENT)
			{
				_children[index] = NULL;
			}
			return true;
		}
		else if(it->second == NULL)
		{
			if(type == TYPE_DEFAULT)
			{
				_children.erase(it);
			}
			return true;
		}
	}
	return false;
}

Sprite* SpriteGroup::getSprite(unsigned int x, unsigned int y)
{
	if(x < _groupWidth && y < _groupHeight)
	{
		unsigned int index = x * _groupWidth + y;
		std::map<unsigned int, Sprite*>::const_iterator it = _children.find(index);
		if(it == _children.end())
		{
			return static_cast<Sprite*>(this);
		}
		else
		{
			Sprite* s = it->second;
			if(s != NULL)
			{
				return s;
			}
		}
	}
	return NULL;
}

bool SpriteGroup::setSprite(unsigned int x, unsigned int y, Sprite* sprite)
{
	if(x < _groupWidth && y < _groupHeight && sprite != NULL && sprite != static_cast<Sprite*>(this))
	{
		unsigned int index = x * _groupWidth + y;
		std::map<unsigned int, Sprite*>::const_iterator it = _children.find(index);
		if(it != _children.end())
		{
			//Remove the Sprite if there's one there already
			Sprite* s = it->second;
			if(s)
			{
				SAFE_RELEASE(s);
			}
		}
		//Set the new Sprite
		sprite->addRef();
		_children[index] = sprite;
		return true;
	}
	return setSpriteType(x, y, sprite == NULL ? TYPE_TRANSPARENT : TYPE_DEFAULT);
}

void SpriteGroup::draw(bool isolateDraw)
{
	SpriteBatch* defBatch = _tileSheet->getSpriteBatch();
	std::vector<SpriteBatch*> processedBatches;

	Vector2 groupSize = Vector2((_width + _horzGap) * _groupWidth, (_height + _vertGap) * _groupHeight);

	//Determine rotation point for entire map
	Vector2 rotationPoint = groupSize * 0.5f;

	Vector3 pos;
	Vector2 size = Vector2(_width, _height);
	float angle = 0.0f;

	const Matrix* vpMatrix = NULL;
	Camera* activeCamera = NULL;

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

		//Get camera
		activeCamera = node->getScene()->getActiveCamera();

		if (isolateDraw && activeCamera)
		{
			//Adjust projection matrix if this draw is specific to this Sprite
			vpMatrix = &(activeCamera->getViewProjectionMatrix());
			defBatch->setProjectionMatrix(*vpMatrix);
		}
	}
	else
	{
		pos = Vector3::zero();
	}

	//Offset position
	pos.x += _x;
	pos.y += _y;

	//Some defaults
	Vector2 defSize(size);
	if ((_flip & FLIP_HORZ) == FLIP_HORZ)
	{
		defSize.x = -defSize.x;
	}
	if ((_flip & FLIP_VERT) == FLIP_VERT)
	{
		defSize.y = -defSize.y;
	}
	Rectangle defSrc = _defaultTileInUse ? _defaultTile : _frame;

	std::map<unsigned int, Sprite*>::const_iterator it = _children.begin();
	bool noChildren = it == _children.end();

	//Prepare for draw loop
	if (isolateDraw)
	{
		defBatch->start();
	}

	//Draw loop
	for(unsigned int x = 0; x < _groupWidth; x++)
	{
		Vector3 sTempPos = pos;
		sTempPos.x += x * (size.x + _horzGap);
		for(unsigned int y = 0; y < _groupHeight; y++)
		{
			sTempPos.y += y ? (size.y + _vertGap) : 0;

			Vector3 sPos = sTempPos;
			bool drawDefault = true;
			if(!noChildren)
			{
				unsigned int index = x * _groupWidth + y;
				if(it->first == index)
				{
					Sprite* s = it->second;
					noChildren = ++it == _children.end();

					if(s)
					{
						//Custom sprite
						drawDefault = false;

						SpriteBatch* batch = s->_tileSheet->getSpriteBatch();

						if(isolateDraw)
						{
							//Is this a new SpriteBatch?
							std::vector<SpriteBatch*>::iterator bit = std::find(processedBatches.begin(), processedBatches.end(), batch);
							if(bit == processedBatches.end())
							{
								//New SpriteBatch
								processedBatches.push_back(batch);
								if(vpMatrix)
								{
									batch->setProjectionMatrix(*vpMatrix);
								}
								batch->start();
							}
						}

						//TODO: figure out how to draw multi-sized sprites using this (AKA< don't use size... but then how do we process the values? Keep a list of each one like the SpriteBatches?)

						//Prepare for drawing
						Vector2 sSize = size;

						//Handle flip
						if ((s->_flip & FLIP_HORZ) == FLIP_HORZ)
						{
							sPos.x += sSize.x;
							sSize.x = -sSize.x;
						}
						if ((s->_flip & FLIP_VERT) == FLIP_VERT)
						{
							sPos.y += sSize.y;
							sSize.y = -sSize.y;
						}

						//Draw
						batch->draw(sPos, s->_defaultTileInUse ? s->_defaultTile : s->_frame, sSize, s->_tint, rotationPoint, angle);
					}
					else
					{
						//Transparent, skip
						continue;
					}
				}
			}

			//If nothing was drawn, draw default
			if(drawDefault)
			{
				if ((_flip & FLIP_HORZ) == FLIP_HORZ)
				{
					sPos.x -= defSize.x;
				}
				if ((_flip & FLIP_VERT) == FLIP_VERT)
				{
					sPos.y -= defSize.y;
				}
				defBatch->draw(sPos, defSrc, defSize, _tint, rotationPoint, angle);
			}
		}
	}

	if (isolateDraw)
	{
		defBatch->finish();
		for(std::vector<SpriteBatch*>::iterator bit = processedBatches.begin(); bit != processedBatches.end(); bit++)
		{
			(*bit)->finish();
		}
	}
}

Sprite* SpriteGroup::clone(NodeCloneContext &context)
{
	SpriteGroup* copy = create(getId(), getGroupWidth(), getGroupHeight(), getTileSheet());
	cloneInto(copy, context);
	return static_cast<Sprite*>(copy);
}

void SpriteGroup::cloneInto(SpriteGroup* group, NodeCloneContext &context) const
{
	//Clone Sprite values
	Sprite::cloneInto(static_cast<Sprite*>(group), context);

	//Clone values
	group->_groupWidth = _groupWidth;
	group->_groupHeight = _groupHeight;
	group->_horzGap = _horzGap;
	group->_vertGap = _vertGap;

	//Clone children
	for(std::map<unsigned int, Sprite*>::const_iterator it = _children.begin(); it != _children.end(); it++)
	{
		unsigned int index = it->first;
		if(Sprite* s = it->second)
		{
			//Complex, we don't want to bring over unrelated Nodes...
			if(Node* node = s->getNode())
			{
				//Search through nodes
				Node* clonedNode = context.findClonedNode(node);
				if(Sprite* sc = clonedNode->getSprite())
				{
					//Found it, just reference it and move on
					sc->addRef();
					group->_children[index] = sc;
				}
				else
				{
					//Just clone the node to save us the trouble
					group->_children[index] = s->clone(context);
				}
			}
			else
			{
				//Ok, dodged a bullet. We can just duplicate the Sprite
				group->_children[index] = s->clone(context);
			}
		}
		else
		{
			//Easy, set a NULL
			group->_children[index] = NULL;
		}
	}
}

}