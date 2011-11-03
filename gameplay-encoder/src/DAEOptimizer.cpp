#include "DAEOptimizer.h"

#include <algorithm>

#include "StringUtil.h"

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
    if (element && element->getElementType() == COLLADA_TYPE::NODE)
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
    for (std::list<domChannelRef>::iterator i = channels.begin(); i != channels.end(); i++)
    {
        moveChannelAndSouresToAnimation(*i, animation);
    }

    // Clean up the empty animations
    deleteEmptyAnimations();
}

void DAEOptimizer::getAnimationChannels(const domNodeRef& node, std::list<domChannelRef>& channels)
{
    assert(node->getId());
    std::string nodeIdSlash (node->getId());
    nodeIdSlash.append("/");

    domCOLLADA* root = (domCOLLADA*)node->getDocument()->getDomRoot();

    domLibrary_animations_Array& animationLibrary = root->getLibrary_animations_array();
    size_t animationLibraryCount = animationLibrary.getCount();
    for (size_t i = 0; i < animationLibraryCount; i++)
    {
        domLibrary_animationsRef& animationsRef = animationLibrary.get(i);
        domAnimation_Array& animationArray = animationsRef->getAnimation_array();
        size_t animationCount = animationArray.getCount();
        for (size_t j = 0; j < animationCount; j++)
        {
            domAnimationRef& animationRef = animationArray.get(j);
            domChannel_Array& channelArray = animationRef->getChannel_array();
            size_t channelArrayCount = channelArray.getCount();
            for (size_t k = 0; k < channelArrayCount; k++)
            {
                domChannelRef& channel = channelArray.get(k);
                const char* target = channel->getTarget();

                // TODO: Assumes only one target per channel?
                if (startsWith(target, nodeIdSlash.c_str()))
                {
                    channels.push_back(channel);
                }
            }
        }
    }

    // Recursively do the same for all nodes
    daeTArray<daeSmartRef<daeElement>> children;
    node->getChildren(children);
    size_t childCount = children.getCount();
    for (size_t i = 0; i < childCount; i++)
    {
        daeElementRef childElement = children[i];
        if (childElement->getElementType() == COLLADA_TYPE::NODE)
        {
            domNodeRef childNode = daeSafeCast<domNode>(childElement);
            getAnimationChannels(childNode, channels);
        }
    }
}

void DAEOptimizer::deleteEmptyAnimations()
{
    std::list<domAnimationRef> animations;
    
    // Get the list of empty animations
    domLibrary_animations_Array& animationLibrary = _dom->getLibrary_animations_array();
    size_t animationLibraryCount = animationLibrary.getCount();
    for (size_t i = 0; i < animationLibraryCount; i++)
    {
        domLibrary_animationsRef& animationsRef = animationLibrary.get(i);
        domAnimation_Array& animationArray = animationsRef->getAnimation_array();
        size_t animationCount = animationArray.getCount();
        for (size_t j = 0; j < animationCount; j++)
        {
            domAnimationRef& animation = animationArray.get(j);
            if (isEmptyAnimation(animation))
            {
                animations.push_back(animation);
            }
        }
    }

    // Delete all of the empty animations
    for (std::list<domAnimationRef>::iterator i = animations.begin(); i != animations.end(); i++)
    {
        daeElement::removeFromParent(*i);
    }
}