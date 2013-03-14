#ifndef SPRITE_H_
#define SPRITE_H_

#include "Ref.h"
#include "AnimationTarget.h"

namespace gameplay
{

class Node;

/**
 * Defines a 2D game visual that may be animated.
 */
class Sprite : public AnimationTarget, public Ref
{

public:

	//TODO

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

protected:

	/**
     * Constructor.
     */
    Sprite(const char* id);

    /**
     * Destructor.
     */
    virtual ~Sprite();

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

	//TODO
};

}

#endif