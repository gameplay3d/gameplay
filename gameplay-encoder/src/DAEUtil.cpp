#include "Base.h"
#include "DAEUtil.h"
#include "StringUtil.h"
#include "DAEChannelTarget.h"
#include "Vector4.h"

namespace gameplay
{

/**
 * Returns the index of the skeleton in skeletonArray that points to the given node.
 * 
 * @param skeletonArray The array of skeletons to search.
 * @param node The target node.
 * 
 * @return The index in skeletonArray or -1 if not found.
 */
int getIndex(const domInstance_controller::domSkeleton_Array& skeletonArray, const domNodeRef& node);

void getAnimationChannels(const domNodeRef& node, std::list<domChannelRef>& channels, bool recursive)
{
    assert(node->getId());
    std::string nodeIdSlash (node->getId());
    nodeIdSlash.append("/");

    domCOLLADA* root = (domCOLLADA*)node->getDocument()->getDomRoot();

    domLibrary_animations_Array& animationLibrary = root->getLibrary_animations_array();
    size_t animationLibraryCount = animationLibrary.getCount();
    for (size_t i = 0; i < animationLibraryCount; ++i)
    {
        domLibrary_animationsRef& animationsRef = animationLibrary.get(i);
        domAnimation_Array& animationArray = animationsRef->getAnimation_array();
        size_t animationCount = animationArray.getCount();
        for (size_t j = 0; j < animationCount; ++j)
        {
            domAnimationRef& animationRef = animationArray.get(j);
            domChannel_Array& channelArray = animationRef->getChannel_array();
            size_t channelArrayCount = channelArray.getCount();
            for (size_t k = 0; k < channelArrayCount; ++k)
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

    if (recursive)
    {
        // Recursively do the same for all nodes
        daeTArray< daeSmartRef<daeElement>> children;
        node->getChildren(children);
        size_t childCount = children.getCount();
        for (size_t i = 0; i < childCount; ++i)
        {
            daeElementRef childElement = children[i];
            if (childElement->getElementType() == COLLADA_TYPE::NODE)
            {
                domNodeRef childNode = daeSafeCast<domNode>(childElement);
                getAnimationChannels(childNode, channels);
            }
        }
    }
}

void getJointNames(const domSourceRef source, std::vector<std::string>& list)
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

void getJointNames(const domSkin* skin, std::vector<std::string>& list)
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

void getStartAndEndTime(const domChannelRef& channel, float* startTime, float* endTime)
{
    domSourceRef source = getInputSource(channel);
    const domFloat_arrayRef floatArray = source->getFloat_array();
    const domListOfFloats& floatList = floatArray->getValue();
    assert(floatList.getCount() > 0);
    *startTime = (float)floatList.get(0);
    *endTime = (float)floatList.get(floatList.getCount() - 1);
}

void getStartAndEndTime(const std::list<domChannelRef>& channels, float* startTime, float* endTime)
{
    float minStart = FLT_MAX, maxEnd = 0, start = FLT_MAX, end = 0;
    for (std::list<domChannelRef>::const_iterator i = channels.begin(); i != channels.end(); ++i)
    {
        getStartAndEndTime(*i, &start, &end);
        minStart = std::min(start, minStart);
        maxEnd = std::max(end, maxEnd);
    }
    assert(minStart <= maxEnd);
    *startTime = minStart;
    *endTime = maxEnd;
}

DAEChannelTarget* getChannelTarget(std::list<DAEChannelTarget>& targets, const char* sid)
{
    if (!sid)
        return NULL;

    for (std::list<DAEChannelTarget>::const_iterator i = targets.begin(); i != targets.end(); ++i)
    {
        
    }
    return NULL;
}

void getRotate(Vector4& rotate, float time)
{
    // if property is ANGLE

}

void bakeNodeTransform(domNodeRef& node)
{
    Matrix nodeBakedMatrix;

    daeTArray<daeSmartRef<daeElement>> children;
    node->getChildren(children);
    size_t childCount = children.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        daeElementRef childElement = children[i];
        switch (childElement->getElementType())
        {
            case COLLADA_TYPE::TRANSLATE:
                {
                    domTranslateRef translateNode = daeSafeCast<domTranslate>(childElement);
                    float x = (float)translateNode->getValue().get(0);
                    float y = (float)translateNode->getValue().get(1);
                    float z = (float)translateNode->getValue().get(2);
                    nodeBakedMatrix.translate(x, y, z);
                    break;
                }
            case COLLADA_TYPE::ROTATE:
                {
                    domRotateRef rotateNode = daeSafeCast<domRotate>(childElement);
                    Vector4 rotate;
                    rotate.x = (float)rotateNode->getValue().get(0);
                    rotate.y = (float)rotateNode->getValue().get(1);
                    rotate.z = (float)rotateNode->getValue().get(2);
                    rotate.w = MATH_DEG_TO_RAD((float)rotateNode->getValue().get(3)); // COLLADA uses degrees, gameplay uses radians
                    if (rotate.w != 0.0f)
                    {
                        nodeBakedMatrix.rotate(rotate.x, rotate.y, rotate.z, rotate.w);
                    }
                    break;
                }
            case COLLADA_TYPE::SCALE:
                {
                    domScaleRef scaleNode = daeSafeCast<domScale>(childElement);
                    float x = (float)scaleNode->getValue().get(0);
                    float y = (float)scaleNode->getValue().get(1);
                    float z = (float)scaleNode->getValue().get(2);
                    nodeBakedMatrix.scale(x, y, z);
                    break;
                }
            default:
                break;
        }
    }
    // go through the list backwards because elements will be removed
    for (int i = childCount - 1; i >= 0; --i)
    {
        daeElementRef childElement = children[i];
        switch (childElement->getElementType())
        {
            case COLLADA_TYPE::TRANSLATE:
            case COLLADA_TYPE::ROTATE:
            case COLLADA_TYPE::SCALE:
            case COLLADA_TYPE::EXTRA:
                daeElement::removeFromParent(childElement);
            default:
                break;
        }
    }

    domMatrixRef mat = daeSafeCast<domMatrix>(node->createAndPlaceAt(0, "matrix"));
    mat->setSid("transform");
    domFloat4x4 floatMatrix;
    floatMatrix.setCount(16);
    floatMatrix.set(0, nodeBakedMatrix.m[0]);
    floatMatrix.set(1, nodeBakedMatrix.m[4]);
    floatMatrix.set(2, nodeBakedMatrix.m[8]);
    floatMatrix.set(3, nodeBakedMatrix.m[12]);
    floatMatrix.set(4, nodeBakedMatrix.m[1]);
    floatMatrix.set(5, nodeBakedMatrix.m[5]);
    floatMatrix.set(6, nodeBakedMatrix.m[9]);
    floatMatrix.set(7, nodeBakedMatrix.m[13]);
    floatMatrix.set(8, nodeBakedMatrix.m[2]);
    floatMatrix.set(9, nodeBakedMatrix.m[6]);
    floatMatrix.set(10, nodeBakedMatrix.m[10]);
    floatMatrix.set(11, nodeBakedMatrix.m[14]);
    floatMatrix.set(12, nodeBakedMatrix.m[3]);
    floatMatrix.set(13, nodeBakedMatrix.m[7]);
    floatMatrix.set(14, nodeBakedMatrix.m[11]);
    floatMatrix.set(15, nodeBakedMatrix.m[15]);
    mat->setValue(floatMatrix);
}

void getNodeTransform(const domNodeRef& node, std::list<domChannelRef>& channels, float time, Matrix& dstTransform)
{
    // for each transform element in node
    // modify the transform element if one (or more?) of the channels targets it
    // bake the transform

    std::list<DAEChannelTarget> targets;

    //DAEChannelTarget channelTarget(
    for (std::list<domChannelRef>::const_iterator i = channels.begin(); i != channels.end(); ++i)
    {
        targets.push_back(DAEChannelTarget(*i));
    }
    // TODO sort targets?

    daeTArray<daeSmartRef<daeElement>> children;
    node->getChildren(children);
    size_t childCount = children.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        daeElementRef childElement = children[i];
        switch (childElement->getElementType())
        {
            case COLLADA_TYPE::TRANSLATE:
                {
                    domTranslateRef translateNode = daeSafeCast<domTranslate>(childElement);
                    float x = (float)translateNode->getValue().get(0);
                    float y = (float)translateNode->getValue().get(1);
                    float z = (float)translateNode->getValue().get(2);
                    dstTransform.translate(x, y, z);
                    break;
                }
            case COLLADA_TYPE::ROTATE:
                {
                    domRotateRef rotateNode = daeSafeCast<domRotate>(childElement);
                    //rotateNode->getSid();
                    Vector4 rotate;
                    rotate.x = (float)rotateNode->getValue().get(0);
                    rotate.y = (float)rotateNode->getValue().get(1);
                    rotate.z = (float)rotateNode->getValue().get(2);
                    rotate.w = MATH_DEG_TO_RAD((float)rotateNode->getValue().get(3)); // COLLADA uses degrees, gameplay uses radians

                    if (rotate.w != 0.0f)
                    {
                        dstTransform.rotate(rotate.x, rotate.y, rotate.z, rotate.w);
                    }
                    break;
                }
            case COLLADA_TYPE::SCALE:
                {
                    domScaleRef scaleNode = daeSafeCast<domScale>(childElement);
                    float x = (float)scaleNode->getValue().get(0);
                    float y = (float)scaleNode->getValue().get(1);
                    float z = (float)scaleNode->getValue().get(2);
                    dstTransform.scale(x, y, z);
                    break;
                }
            default:
                break;
        }

    }
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

}
