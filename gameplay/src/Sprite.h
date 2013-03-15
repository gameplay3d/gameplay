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
	 * Creates a new Sprite for drawing.
	 * 
	 * @param id The ID for the new Sprite.
	 * @param tileSheet The TileSheet that represents this Sprite.
	 * 
	 * @return A new Sprite for drawing.
	 * @script{create}
	 */
	static Sprite* create(const char* id, TileSheet* tileSheet);

	/**
     * Gets the identifier for the sprite.
     *
     * @return The sprite identifier.
     */
    const char* getId() const;

	//TODO

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

	//TODO
};

}

#endif