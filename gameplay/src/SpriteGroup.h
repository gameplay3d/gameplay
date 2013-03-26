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
     * The Sprite is transparent.
     */
    static const int TYPE_TRANSPARENT = 0;

	/**
     * The Sprite is the default Sprite defined by the SpriteGroup.
     */
    static const int TYPE_DEFAULT = 1;

	/**
     * The Sprite is child Sprite of this SpriteGroup.
     */
    static const int TYPE_CHILD = 2;

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
	 * Creates a new SpriteGroup for drawing.
	 * 
	 * @param id The ID for the new SpriteGroup.
	 * @param width The width (in number of sprites wide) of the SpriteGroup.
	 * @param height The height (in number of sprites tall) of the SpriteGroup.
	 * @param sprite A Sprite to copy for use as the default sprite for the 
	 *  SpriteGroup. Note: This does not use the Sprite, it mearly copies it.
	 * 
	 * @return A new SpriteGroup for drawing.
	 */
	static SpriteGroup* createFrom(const char* id, unsigned int width, unsigned int height, Sprite* sprite);

	/**
     * Get a named SpriteGroup from its ID.
     *
     * @param id The ID of the SpriteGroup to search for.
     *
     * @return The SpriteGroup with the specified ID, or NULL if one was not found.
     */
	static SpriteGroup* getSpriteGroup(const char* id);

	/**
     * Get the width of the the grid of the SpriteGroup.
     *
     * @return The SpriteGroup width.
     */
	unsigned int getGroupWidth() const;

	/**
     * Get the height of the the grid of the SpriteGroup.
     *
     * @return The SpriteGroup height.
     */
	unsigned int getGroupHeight() const;

	/**
     * Get the horizontal gap between Sprites in this SpriteGroup.
     *
     * @return The horizontal gap between Sprites.
     */
	float getHorzGap() const;

	/**
     * Set the horizontal gap between Sprites in this SpriteGroup.
     *
     * @param gap The horizontal gap between Sprites.
     */
	void setHorzGap(float gap);

	/**
     * Get the vertical gap between Sprites in this SpriteGroup.
     *
     * @return The vertical gap between Sprites.
     */
	float getVertGap() const;

	/**
     * Set the vertical gap between Sprites in this SpriteGroup.
     *
     * @param gap The vertical gap between Sprites.
     */
	void setVertGap(float gap);

	/**
     * Get the type of sprite at the specified location within the SpriteGroup.
	 * 
	 * @param x The X position within the SpriteGroup.
	 * @param y The Y position within the SpriteGroup.
     *
     * @return The sprite type: TYPE_TRANSPARENT, TYPE_DEFAULT, TYPE_CHILD, or -1 on error.
     */
	int getSpriteType(unsigned int x, unsigned int y) const;

	/**
     * Set the type of sprite at the specified location within the SpriteGroup.
	 * 
	 * @param x The X position within the SpriteGroup.
	 * @param y The Y position within the SpriteGroup.
	 * @param type Either TYPE_TRANSPARENT or TYPE_DEFAULT for the specified sprite.
     *
     * @return true on success, false if TYPE_CHILD or any other type of sprite is used, or an error occurs.
     */
	bool setSpriteType(unsigned int x, unsigned int y, int type);

	/**
     * Get the sprite at the specified location within the SpriteGroup.
	 * 
	 * @param x The X position within the SpriteGroup.
	 * @param y The Y position within the SpriteGroup.
     *
     * @return "this" SpriteGroup if TYPE_DEFAULT, the sprite if TYPE_CHILD, or 
	 *  NULL if TYPE_TRANSPARENT or an error occured.
     */
	Sprite* getSprite(unsigned int x, unsigned int y);

	/**
     * Set the sprite at the specified location within the SpriteGroup.
	 * 
	 * @param x The X position within the SpriteGroup.
	 * @param y The Y position within the SpriteGroup.
	 * @param sprite NULL for a transparent sprite, "this" to set to default, or another Sprite to a child.
     *
     * @return true on success, false if an error occurs.
     */
	bool setSprite(unsigned int x, unsigned int y, Sprite* sprite);

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

	unsigned int _groupWidth;
	unsigned int _groupHeight;

	std::map<unsigned int, Sprite*> _children;

protected:

	/**
	 * The horizontal gap between Sprites.
	 */
	float _horzGap;

	/**
	 * The vertical gap between Sprites.
	 */
	float _vertGap;
};

}

#endif