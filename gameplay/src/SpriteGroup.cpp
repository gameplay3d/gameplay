#include "Base.h"
#include "SpriteGroup.h"

namespace gameplay
{

// Global list of active groups
static std::vector<SpriteGroup*> _spriteGroups;

SpriteGroup::SpriteGroup(const char* id)
	: Sprite(id), _groupWidth(1), _groupHeight(1), _horzGap(0), _vertGap(0)
{
}

SpriteGroup::~SpriteGroup()
{
	// Remove self from vector.
    std::vector<SpriteGroup*>::iterator it = std::find(_spriteGroups.begin(), _spriteGroups.end(), this);
    if (it != _spriteGroups.end())
    {
        _spriteGroups.erase(it);
    }
}

SpriteGroup* SpriteGroup::create(const char* id, unsigned int width, unsigned int height, TileSheet* tileSheet)
{
	//TODO
	return NULL;
}

SpriteGroup* SpriteGroup::createFrom(const char* id, unsigned int width, unsigned int height, Sprite* sprite)
{
	//TODO
	return NULL;
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
	//TODO
	return -1;
}

bool SpriteGroup::setSpriteType(unsigned int x, unsigned int y, int type)
{
	//TODO
	return false;
}

Sprite* SpriteGroup::getSprite(unsigned int x, unsigned int y) const
{
	//TODO
	return NULL;
}

bool SpriteGroup::setSprite(unsigned int x, unsigned int y, Sprite* sprite)
{
	//TODO
	return false;
}

void SpriteGroup::draw(bool isolateDraw)
{
	Sprite::draw(isolateDraw); //Temp
}

Sprite* SpriteGroup::clone(NodeCloneContext &context)
{
	SpriteGroup* copy = create(getId(), getGroupWidth(), getGroupHeight(), getTileSheet());

	//Copy Sprite values
	Sprite::cloneInto(static_cast<Sprite*>(copy), context);

	//Copy values
	copy->_groupWidth = _groupWidth;
	copy->_groupHeight = _groupHeight;
	copy->_horzGap = _horzGap;
	copy->_vertGap = _vertGap;

	//TODO: values, nodes (be careful, we don't want to duplicate Sprites and Nodes)

	return static_cast<Sprite*>(copy);
}

}