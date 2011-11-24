
#include "DAEUtil.h"

/**
 * Returns the index of the skeleton in skeletonArray that points to the given node.
 * 
 * @param skeletonArray The array of skeletons to search.
 * @param node The target node.
 * 
 * @return The index in skeletonArray or -1 if not found.
 */
int getIndex(const domInstance_controller::domSkeleton_Array& skeletonArray, const domNodeRef& node);

void getJointNames(const domSourceRef source, std::list<std::string>& list)
{
    // BLENDER used name_array
    const domName_arrayRef& nameArray = source->getName_array();
    if (nameArray.cast())
    {
        domListOfNames& ids = nameArray->getValue();
        size_t jointCount = (size_t)nameArray->getCount();
        for (size_t j = 0; j < jointCount; ++j)
        {
            list.push_back(std::string(ids.get(j)));
        }
    }
    else
    {
        // Seymour used IDREF_array
        const domIDREF_arrayRef& idArray = source->getIDREF_array();
        if (idArray.cast())
        {
            xsIDREFS& ids = idArray->getValue();
            size_t jointCount = (size_t)idArray->getCount();
            for (size_t j = 0; j < jointCount; ++j)
            {
                list.push_back(std::string(ids.get(j).getID()));
            }
        }
    }
}

void getJointNames(const domSkin* skin, std::list<std::string>& list)
{
    const domSkin::domJointsRef& joints = skin->getJoints();
    const domInputLocal_Array& inputArray = joints->getInput_array();
    size_t inputCount = inputArray.getCount();
    for (size_t i = 0; i < inputCount; ++i)
    {
        const domInputLocalRef input = inputArray.get(i);
        const char* semantic = input->getSemantic();
        if (strcmp(semantic, "JOINT") == 0)
        {
            daeElement* sourceElement = input->getSource().getElement();
            if (sourceElement)
            {
                const domSourceRef source = daeSafeCast<domSource>(sourceElement);
                getJointNames(source, list);
            }
        }
    }
}

domSource* getInputSource(const domChannelRef& channel)
{
    daeElement* element = channel->getSource().getElement();
    if (element && element->getElementType() == COLLADA_TYPE::SAMPLER)
    {
        domSampler* sampler = daeSafeCast<domSampler>(element);
        const domInputLocal_Array& inputArray = sampler->getInput_array();
        size_t inputArrayCount = inputArray.getCount();
        for (size_t i = 0; i < inputArrayCount; ++i)
        {
            const domInputLocalRef& input = inputArray.get(i);
            if (strcmp(input->getSemantic(), "INPUT") == 0)
            {
                daeElement* e = input->getSource().getElement();
                if (e && e->getElementType() == COLLADA_TYPE::SOURCE)
                {
                    domSource* source = daeSafeCast<domSource>(e);
                    assert(source);
                    return source;
                }
            }
        }
    }
    return NULL;
}

const domSamplerRef getSampler(const domChannelRef& channel)
{
    const domURIFragmentType& uri = channel->getSource();
    daeElementRef element = uri.getElement();
    if (element && element->getElementType() == COLLADA_TYPE::SAMPLER)
    {
        const domSamplerRef sampler = daeSafeCast<domSampler>(element);
        return sampler;
    }
    // resolve the source manually by searching for the sampler in the animation that the channel is a child of.
    const std::string& id = uri.id();
    const daeElementRef& parent = channel->getParent();
    if (parent && parent->getElementType() == COLLADA_TYPE::ANIMATION)
    {
        const domAnimationRef animation = daeSafeCast<domAnimation>(parent);
        
        const domSampler_Array& samplerArray = animation->getSampler_array();
        size_t count = samplerArray.getCount();
        for (size_t i = 0; i < count; ++i)
        {
            const domSamplerRef& sampler = samplerArray.get(i);
            if (id.compare(sampler->getId()) == 0)
            {
                return sampler;
            }
        }
    }
    return NULL;
}

const domSourceRef getSource(const domInputLocalRef& inputLocal, const domAnimationRef& animation)
{
    const domURIFragmentType& uri = inputLocal->getSource();
    daeElementRef element = uri.getElement();
    if (element && element->getElementType() == COLLADA_TYPE::SAMPLER)
    {
        const domSourceRef source = daeSafeCast<domSource>(element);
        return source;
    }
    // Resolve the URI by searching through the animation's list of sources
    const std::string& id = uri.id();
    const domSource_Array& sourceArray = animation->getSource_array();
    size_t count = sourceArray.getCount();
    for (size_t i = 0; i < count; ++i)
    {
        const domSourceRef source = sourceArray.get(i);
        if (id.compare(source->getId()) == 0)
        {
            return source;
        }
    }
    return NULL;
}

const domName_arrayRef getSourceNameArray(const domSourceRef& source)
{
    const domName_arrayRef& nameArray = source->getName_array();
    if (nameArray)
    {
        return nameArray;
    }
    daeTArray<daeSmartRef<daeElement> > children;
    source->getChildren(children);
    size_t childCount = children.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        const daeElementRef element = children.get(i);
        if (element->getElementType() == COLLADA_TYPE::NAME_ARRAY)
        {
            return daeSafeCast<domName_array>(element);
        }
    }
    return NULL;
}

const domInstance_controller::domSkeletonRef getSkeleton(const domInstance_controllerRef& instanceController)
{
    domInstance_controller::domSkeleton_Array& skeletonArray = instanceController->getSkeleton_array();
    size_t count = skeletonArray.getCount();
    if (count == 0)
    {
        return NULL;
    }
    if (count == 1)
    {
        return skeletonArray.get(0);
    }
    // Maya sometimes outputs multiple skeleton elements.
    // Find the skeleton element that points to the root most node.
    const domInstance_controller::domSkeletonRef& currentSkeleton = skeletonArray.get(0);
    const daeElementRef element = currentSkeleton->getValue().getElement();
    if (element && element->getElementType() == COLLADA_TYPE::NODE)
    {
        domNode* node = daeSafeCast<domNode>(element);
        int index = 0;
        bool loop = true;
        do
        {
            daeElementRef parent = node->getParent();
            if (parent && parent->getElementType() == COLLADA_TYPE::NODE)
            {
                domNodeRef parentNode = daeSafeCast<domNode>(parent);
                int result = getIndex(skeletonArray, parentNode);
                if (result >= 0)
                {
                    index = result;
                }
                node = parentNode;
            }
            else
            {
                loop = false;
            }
        } while (loop);
        if (index >= 0)
        {
            return skeletonArray.get(index);
        }
    }
    return NULL;
}

bool equalKeyTimes(const domSource* s1, const domSource* s2)
{
    // TODO: shouldn't assume that the source has a float array.
    const domFloat_arrayRef& f1 = s1->getFloat_array();
    const domFloat_arrayRef& f2 = s2->getFloat_array();
    if (f1->getCount() == f2->getCount())
    {
        const domListOfFloats& list1 = f1->getValue();
        const domListOfFloats& list2 = f2->getValue();

        size_t count = (size_t)f1->getCount();
        for (size_t i = 0; i < count; ++i)
        {
            if (list1.get(i) != list2.get(i))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool equalKeyTimes(const domChannelRef& c1, const domChannelRef& c2)
{
    domSource* s1 = getInputSource(c1);
    domSource* s2 = getInputSource(c2);
    assert(s1);
    assert(s2);
    return equalKeyTimes(s1, s2);
}

void moveChannelAndSouresToAnimation(domChannelRef& channel, domAnimationRef& animation)
{
    assert(channel);
    assert(animation);

    daeElement::removeFromParent(channel);
    animation->add(channel); // move channel

    daeElementRef element = channel->getSource().getElement();
    if (element)
    {
        domSamplerRef sampler = daeSafeCast<domSampler>(element);

        domInputLocal_Array& inputArray = sampler->getInput_array();
        size_t inputArrayCount = inputArray.getCount();
        for (size_t i = 0; i < inputArrayCount; ++i)
        {
            inputArray = sampler->getInput_array();
            const domInputLocalRef& input = inputArray.get(i);
            daeElementRef element = input->getSource().getElement();
            if (element && element->getElementType() == COLLADA_TYPE::SOURCE)
            {
                domSourceRef source = daeSafeCast<domSource>(element);
                assert(source);
                daeElement::removeFromParent(source);
                animation->add(source); // move source
            }
        }
        daeElement::removeFromParent(sampler);
        animation->add(sampler); // move sampler
    }
}

bool isEmptyAnimation(domAnimationRef& animation)
{
    return animation->getAnimation_array().getCount() == 0 &&
           animation->getChannel_array().getCount() == 0 &&
           animation->getSampler_array().getCount() == 0 &&
           animation->getSource_array().getCount() == 0;
}

int getIndex(const domInstance_controller::domSkeleton_Array& skeletonArray, const domNodeRef& node)
{
    const std::string nodeId = node->getId();
    size_t count = skeletonArray.getCount();
    for (size_t i = 0; i < count; ++i)
    {
        const domInstance_controller::domSkeletonRef& skeleton = skeletonArray.get(i);
        daeElementRef element = skeleton->getValue().getElement();
        if (element->getElementType() == COLLADA_TYPE::NODE)
        {
            domNodeRef targetNode = daeSafeCast<domNode>(element);
            if (nodeId.compare(targetNode->getId()) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}
