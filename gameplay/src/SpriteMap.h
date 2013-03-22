#ifndef SPRITEMAP_H_
#define SPRITEMAP_H_

#include "Sprite.h"

namespace gameplay
{

/**
 * Defines a 2D map of Sprites.
 */
class SpriteMap : public Sprite
{

public:

	//TODO

protected:

	/**
     * Constructor.
     */
    SpriteMap(const char* id);

    /**
     * Destructor.
     */
    virtual ~SpriteMap();

private:

    SpriteMap(const SpriteMap& copy);
    SpriteMap& operator=(const SpriteMap&);

	//TODO
};

}

#endif