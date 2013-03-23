#include "Base.h"
#include "SpriteMap.h"

namespace gameplay
{

// Global list of active maps
static std::vector<SpriteMap*> _spriteMaps;

SpriteMap::SpriteMap(const char* id)
	: Sprite(id)
{
}

SpriteMap::~SpriteMap()
{
	// Remove self from vector.
    std::vector<SpriteMap*>::iterator it = std::find(_spriteMaps.begin(), _spriteMaps.end(), this);
    if (it != _spriteMaps.end())
    {
        _spriteMaps.erase(it);
    }
}

SpriteMap* SpriteMap::create(const char* id, unsigned int width, unsigned int height, TileSheet* tileSheet)
{
	//TODO
	return NULL;
}

SpriteMap* SpriteMap::getSpriteMap(const char* id)
{
	GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<SpriteMap*>::const_iterator it;
    for (it = _spriteMaps.begin(); it != _spriteMaps.end(); ++it)
    {
        SpriteMap* sm = *it;
        GP_ASSERT(sm);
		if (sm->_id == id)
        {
            return sm;
        }
    }
    return NULL;
}

void SpriteMap::draw(bool isolateDraw)
{
	Sprite::draw(isolateDraw); //Temp
}

}