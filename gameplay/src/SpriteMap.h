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

	/**
	 * Creates a new SpriteMap for drawing.
	 * 
	 * @param id The ID for the new SpriteMap.
	 * @param width The width (in number of tiles) of the SpriteMap.
	 * @param height The height (in number of tiles) of the SpriteMap.
	 * @param tileSheet The TileSheet that represents the tiles for this SpriteMap.
	 * 
	 * @return A new SpriteMap for drawing.
	 * @script{create}
	 */
	static SpriteMap* create(const char* id, unsigned int width, unsigned int height, TileSheet* tileSheet);

	//TODO: create function for using a Properties (takes a url to the properties)

	/**
     * Get a named SpriteMap from its ID.
     *
     * @param id The ID of the SpriteMap to search for.
     *
     * @return The SpriteMap with the specified ID, or NULL if one was not found.
     */
	static SpriteMap* getSpriteMap(const char* id);

	//TODO

	/**
	 * Draw the sprite map.
	 *
	 * @param isolateDraw If each draw call is unique and can't be batched. Batched calls 
	 *  have to be controlled by developer and are not handled internally. Technically, if 
	 *  this is true, this just calls start and finish on the TileSheet.
	 */
	virtual void draw(bool isolateDraw = true);

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