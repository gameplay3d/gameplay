#ifndef CLONECONTEXT_H_
#define CLONECONTEXT_H_

#include <map>

namespace gameplay
{
    class Animation;
    class Node;

/**
 * CloneContext represents the context data that is kept when cloning a node.
 * 
 * The CloneContext is used to make sure objects don't get cloned twice.
 */
class CloneContext
{
public:

    /**
     * Constructor.
     */
    CloneContext();

    /**
     * Destructor.
     */
    ~CloneContext();

    /**
     * Finds the cloned animation of the given animation or NULL if this animation was not registered with this context.
     * 
     * @param animation The animation to search for the cloned copy of.
     * 
     * @return The cloned animation or NULL if not found.
     */
    Animation* findClonedAnimation(const Animation* animation);

    /**
     * Registers the cloned animation with this context so that it doesn't get cloned twice.
     * 
     * @param original The pointer to the original animation.
     * @param clone The pointer to the cloned animation.
     */
    void registerClonedAnimation(const Animation* original, Animation* clone);

    /**
     * Finds the cloned node of the given node or NULL if this node was not registered with this context.
     * 
     * @param node The node to search for the cloned copy of.
     * 
     * @return The cloned node or NULL if not found.
     */
    Node* findClonedNode(const Node* node);

    /**
     * Registers the cloned node with this context so that it doens't get cloned twice.
     * 
     * @param original The pointer to the original node.
     * @param clone The pointer to the cloned node.
     */
    void registerClonedNode(const Node* original, Node* clone);

private:
    
    /**
     * Hidden copy constructor.
     */
    CloneContext(const CloneContext&);

    /**
     * Hidden copy assignment operator.
     */
    CloneContext& operator=(const CloneContext&);

private:
    typedef std::map<const Animation*, Animation*> AnimationMap;
    typedef std::map<const Node*, Node*> NodeMap;

    AnimationMap _clonedAnimations;
    NodeMap _clonedNodes;
};

}

#endif
