#ifndef SPRITEGROUP_H_
#define SPRITEGROUP_H_

#include "Sprite.h"

namespace gameplay
{

/**
 * Defines a group of Sprites arranged in a 2D grid.
 */
class SpriteGroup : public Sprite
{

public:

	/**
	 * Creates a new SpriteGroup for drawing.
	 * 
	 * @param id The ID for the new SpriteGroup.
	 * @param width The width (in number of sprites wide) of the SpriteGroup.
	 * @param height The height (in number of sprites tall) of the SpriteGroup.
	 * @param tileSheet The TileSheet that represents the tiles for this SpriteGroup.
	 * 
	 * @return A new SpriteGroup for drawing.
	 * @script{create}
	 */
	static SpriteGroup* create(const char* id, unsigned int width, unsigned int height, TileSheet* tileSheet);

	//TODO: create function for using a Properties (takes a url to the properties)

	/**
     * Get a named SpriteGroup from its ID.
     *
     * @param id The ID of the SpriteGroup to search for.
     *
     * @return The SpriteGroup with the specified ID, or NULL if one was not found.
     */
	static SpriteGroup* getSpriteGroup(const char* id);

	//TODO

	/**
	 * Draw the sprite group.
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
    SpriteGroup(const char* id);

    /**
     * Destructor.
     */
    virtual ~SpriteGroup();

	/**
     * @see Sprite::clone
     */
    virtual Sprite* clone(NodeCloneContext &context);

private:

    SpriteGroup(const SpriteGroup& copy);
    SpriteGroup& operator=(const SpriteGroup&);

	//TODO
};

}

#endif