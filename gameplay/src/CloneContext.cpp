#include "CloneContext.h"

namespace gameplay
{

CloneContext::CloneContext()
{
    
}

CloneContext::~CloneContext()
{

}

Animation* CloneContext::findClonedAnimation(const Animation* animation)
{
    AnimationMap::iterator it = _clonedAnimations.find(animation);
    return it != _clonedAnimations.end() ? it->second : NULL;
}

void CloneContext::registerClonedAnimation(const Animation* original, Animation* clone)
{
    _clonedAnimations[original] = clone;
}

Node* CloneContext::findClonedNode(const Node* node)
{
    NodeMap::iterator it = _clonedNodes.find(node);
    return it != _clonedNodes.end() ? it->second : NULL;
}

void CloneContext::registerClonedNode(const Node* original, Node* clone)
{
    _clonedNodes[original] = clone;
}

}
