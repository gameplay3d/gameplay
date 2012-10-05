#include "Base.h"
#include "DAEOptimizer.h"

namespace gameplay
{

DAEOptimizer::DAEOptimizer(domCOLLADA* dom)
{
    _dom = dom;
}

DAEOptimizer::~DAEOptimizer()
{   
}

void DAEOptimizer::combineAnimations(const std::string& nodeId, const std::string& animationId)
{
    std::list<domChannelRef> channels;

    daeSIDResolver resolver(_dom, nodeId.c_str());
    daeElement* element = resolver.getElement();
    if (element && element->typeID() == COLLADA_TYPE::NODE)
    {
        domNodeRef node = daeSafeCast<domNode>(resolver.getElement());
        getAnimationChannels(node, channels);
    }

    // Get the <library_animations>
    domLibrary_animations_Array& animationsLibraryArray = _dom->getLibrary_animations_array();
    assert(animationsLibraryArray.getCount() > 0);
    domLibrary_animationsRef& animationsLibrary = animationsLibraryArray.get(0);

    // Add a new animation
    domAnimationRef animation = daeSafeCast<domAnimation>(animationsLibrary->createAndPlace("animation"));
    assert(animation);
    animation->setId(animationId.c_str());
    // TODO: Make sure that there doesn't already exist an animation with this ID.

    // Move each of the channels to this animation
    for (std::list<domChannelRef>::iterator i = channels.begin(); i != channels.end(); ++i)
    {
        moveChannelAndSouresToAnimation(*i, animation);
    }

    // Clean up the empty animations
    deleteEmptyAnimations();
}

void DAEOptimizer::deleteEmptyAnimations()
{
    std::list<domAnimationRef> animations;
    
    // Get the list of empty animations
    domLibrary_animations_Array& animationLibrary = _dom->getLibrary_animations_array();
    size_t animationLibraryCount = animationLibrary.getCount();
    for (size_t i = 0; i < animationLibraryCount; ++i)
    {
        domLibrary_animationsRef& animationsRef = animationLibrary.get(i);
        domAnimation_Array& animationArray = animationsRef->getAnimation_array();
        size_t animationCount = animationArray.getCount();
        for (size_t j = 0; j < animationCount; ++j)
        {
            domAnimationRef& animation = animationArray.get(j);
            if (isEmptyAnimation(animation))
            {
                animations.push_back(animation);
            }
        }
    }

    // Delete all of the empty animations
    for (std::list<domAnimationRef>::iterator i = animations.begin(); i != animations.end(); ++i)
    {
        daeElement::removeFromParent(*i);
    }
}

}
