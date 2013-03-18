#ifndef SPRITE_H_
#define SPRITE_H_

#include "TileSheet.h"

namespace gameplay
{

class Node;

/**
 * Defines a 2D game visual that may be animated.
 */
class Sprite : public AnimationTarget, public Ref
{
	friend class Node;

public:

	/**
     * The Sprite should not be flipped.
     */
    static const int FLIP_NONE = 0;

	/**
     * The Sprite should be flipped horizontally.
     */
    static const int FLIP_HORZ = 1;

	/**
     * The Sprite should be flipped vertically.
     */
    static const int FLIP_VERT = 2;

	/**
	 * Creates a new Sprite for drawing.
	 * 
	 * @param id The ID for the new Sprite.
	 * @param tileSheet The TileSheet that represents this Sprite.
	 * 
	 * @return A new Sprite for drawing.
	 * @script{create}
	 */
	static Sprite* create(const char* id, TileSheet* tileSheet);

	//TODO: create from properties so animations can be loaded

	/**
     * Gets the identifier for the sprite.
     *
     * @return The sprite identifier.
     */
    const char* getId() const;

	/**
     * Gets the orientation of the sprite.
     *
     * @return The sprite's orientation.
     */
	int getFlip() const;

	/**
	 * Sets the orientation of the sprite.
	 * 
	 * @param flip The sprite orientation.
	 */
	void setFlip(int flip);

	/**
     * Gets the default tile that the sprite will use.
     *
     * @return The sprite's default tile.
     */
	const Rectangle& getDefaultTile() const;

	/**
	 * Sets the default tile that the sprite will use.
	 * 
	 * @param tile The sprite's default tile.
	 */
	void setDefaultTile(const Rectangle& tile);

	/**
     * Gets the untransformed size the sprite will be drawn at.
     *
     * @return The sprite's untransformed size.
     */
	Vector2 getSpriteSize() const;

	/**
     * Gets the untransformed width the sprite will be drawn at.
     *
     * @return The sprite's untransformed width.
     */
	float getSpriteWidth() const;

	/**
     * Gets the untransformed height the sprite will be drawn at.
     *
     * @return The sprite's untransformed height.
     */
	float getSpriteHeight() const;

	/**
	 * Sets the untransformed size the sprite will be drawn at.
	 * 
	 * @param size The untransformed size the sprite will be drawn at.
	 */
	void setSpriteSize(const Vector2& size);

	/**
	 * Sets the untransformed size the sprite will be drawn at.
	 * 
	 * @param width The untransformed width the sprite will be drawn at.
	 * @param height The untransformed height the sprite will be drawn at.
	 */
	void setSpriteSize(float width, float height);

	/**
     * Gets the untransformed offset from the world origin the sprite will be drawn at.
	 *
	 * The world origin is the center of the game window. If the offset is set to be the 
	 * negative half-screen size, then the sprite shows up in the lower-left of the screen.
     *
     * @return The sprite's untransformed offset.
     */
	Vector2 getSpriteOffset() const;

	/**
     * Gets the untransformed X offset from the world origin the sprite will be drawn at.
     *
     * @return The sprite's untransformed X offset.
	 * 
	 * @see Sprite::getSpriteOffset
     */
	float getSpriteOffsetX() const;

	/**
     * Gets the untransformed Y offset from the world origin the sprite will be drawn at.
     *
     * @return The sprite's untransformed Y offset.
	 * 
	 * @see Sprite::getSpriteOffset
     */
	float getSpriteOffsetY() const;

	/**
	 * Sets the untransformed offset from the world origin the sprite will be drawn at.
	 * 
	 * The world origin is the center of the game window. If the offset is set to be the 
	 * negative half-screen size, then the sprite shows up in the lower-left of the screen.
	 * 
	 * @param offset The sprite's untransformed offset.
	 */
	void setSpriteOffset(const Vector2& offset);

	/**
	 * Sets the untransformed offset from the world origin the sprite will be drawn at.
	 * 
	 * The world origin is the center of the game window. If the offset is set to be the 
	 * negative half-screen size, then the sprite shows up in the lower-left of the screen.
	 * 
	 * @param x The sprite's untransformed X offset.
	 * @param y The sprite's untransformed Y offset.
	 */
	void setSpriteOffset(float x, float y);

	/**
	 * Sets the untransformed X offset from the world origin the sprite will be drawn at.
	 * 
	 * The world origin is the center of the game window. If the offset is set to be the 
	 * negative half-screen size, then the sprite shows up in the lower-left of the screen.
	 * 
	 * @param value The sprite's untransformed X offset.
	 */
	void setSpriteOffsetX(float value);

	/**
	 * Sets the untransformed Y offset from the world origin the sprite will be drawn at.
	 * 
	 * The world origin is the center of the game window. If the offset is set to be the 
	 * negative half-screen size, then the sprite shows up in the lower-left of the screen.
	 * 
	 * @param value The sprite's untransformed Y offset.
	 */
	void setSpriteOffsetY(float value);

	//TODO: tint

	//TODO: parallax effect (so layers don't always move when at different depths) //Only needed if camera is not orthographic

	//TODO: scaling based on depth (so layers, even when they are the same size, aren't rendered at different sizes because of different depths) //Only needed if camera is not orthographic

	/**
     * Gets the Node that holds the Sprite.
     *
     * @return The sprite's owning Node.
     */
	Node* getNode() const;

	/**
	 * Sets the Node the holds the Sprite.
	 * 
	 * @param node The node that holds the Sprite.
	 */
	void setNode(Node* node);

	/**
     * Gets the tile sheet that the spritec draws.
     *
     * @return The sprite's tile sheet.
     */
	TileSheet* getTileSheet();

	/**
	 * Draw the sprite.
	 *
	 * @param isolateDraw If each draw call is unique and can't be batched. Batched calls 
	 *  have to be controlled by developer and are not handled internally. Technically, if 
	 *  this is true, this just calls start and finish on the TileSheet's SpriteBatch.
	 */
	virtual void draw(bool isolateDraw = true);

	/**
     * @see AnimationTarget::getAnimationPropertyComponentCount
     */
    unsigned int getAnimationPropertyComponentCount(int propertyId) const;

    /**
     * @see AnimationTarget::getAnimationProperty
     */
    void getAnimationPropertyValue(int propertyId, AnimationValue* value);

    /**
     * @see AnimationTarget::setAnimationProperty
     */
    void setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight = 1.0f);

protected:

	/**
     * Constructor.
     */
    Sprite(const char* id);

    /**
     * Destructor.
     */
    virtual ~Sprite();

	/**
     * Gets the current animation frame that will be displayed.
     *
     * @return The actual source rectangle of what will be drawn for that sprite.
     */
	const Rectangle& getCurrentAnimationFrame() const;

	/**
     * Clones the sprite and returns a new sprite.
     * 
     * @param context The clone context.
     * @return The new cloned sprite.
     */
    virtual Sprite* clone(NodeCloneContext &context);

	/**
     * Copies the data from this Sprite into the given sprite.
     * 
     * @param sprite The sprite to copy the data to.
     * @param context The clone context.
     */
    void cloneInto(Sprite* sprite, NodeCloneContext &context) const;

	//TODO

private:

	/**
     * Hidden copy constructor.
     */
    Sprite(const Sprite& copy);

    /**
     * Hidden copy assignment operator.
     */
    Sprite& operator=(const Sprite&);

protected:

	/**
     * The Sprite's ID.
     */ 
    std::string _id;

	/**
	 * The Sprite's Node that it uses to position itself.
	 */
	Node* _node;

	/**
	 * The Sprite's TileSheet that represents what will be drawn.
	 */
	TileSheet* _tileSheet;

	/**
	 * Bitwise definition of any "flip" the Sprite has.
	 */
	int _flip;

	/**
	 * The default tile the sprite will use.
	 */
	Rectangle _defaultTile;

	/**
	 * The untransformed width of the sprite.
	 */
	float _width;

	/**
	 * The untransformed height of the sprite.
	 */
	float _height;

	/**
	 * The untransformed X offset of the sprite.
	 */
	float _x;

	/**
	 * The untransformed Y offset of the sprite.
	 */
	float _y;

	/**
	 * The actual source Rectangle of the TileSheet that will be drawn.
	 */
	Rectangle _frame;

	/**
	 * Use the default tile instead of animation (usually means that animation is not in use)
	 */
	bool _defaultTileInUse;

	//TODO
};

}

#endif