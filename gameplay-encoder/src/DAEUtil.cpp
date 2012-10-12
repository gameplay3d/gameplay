#include <set>
#include "Base.h"
#include "DAEUtil.h"
#include "StringUtil.h"

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
static int getIndex(const domInstance_controller::domSkeleton_Array& skeletonArray, const domNodeRef& node);

/**
 * Gets all of the animation channels that target the given node and appends them to the list.
 * 
 * @param animationRef The animation to search in.
 * @param nodeIdSlash The node's id with a forward slash appended to it.
 * @param channels The list of channels to append to.
 */
static void getAnimationChannels(const domAnimationRef& animationRef, const std::string& nodeIdSlash, std::list<domChannelRef>& channels);

void getAnimationChannels(const domNodeRef& node, std::list<domChannelRef>& channels)
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
            getAnimationChannels(animationRef, nodeIdSlash, channels);
        }
    }

    // Recursively do the same for all nodes
    daeTArray< daeSmartRef<daeElement> > children;
    node->getChildren(children);
    size_t childCount = children.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        daeElementRef childElement = children[i];
        if (childElement->typeID() == COLLADA_TYPE::NODE)
        {
            domNodeRef childNode = daeSafeCast<domNode>(childElement);
            getAnimationChannels(childNode, channels);
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
    if (element && element->typeID() == COLLADA_TYPE::SAMPLER)
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
                if (e && e->typeID() == COLLADA_TYPE::SOURCE)
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
    if (element && element->typeID() == COLLADA_TYPE::SAMPLER)
    {
        const domSamplerRef sampler = daeSafeCast<domSampler>(element);
        return sampler;
    }
    // resolve the source manually by searching for the sampler in the animation that the channel is a child of.
    const std::string& id = uri.id();
    const daeElementRef& parent = channel->getParent();
    if (parent && parent->typeID() == COLLADA_TYPE::ANIMATION)
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
    if (element && element->typeID() == COLLADA_TYPE::SAMPLER)
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
        if (element->typeID() == COLLADA_TYPE::NAME_ARRAY)
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
    if (element && element->typeID() == COLLADA_TYPE::NODE)
    {
        domNode* node = daeSafeCast<domNode>(element);
        int index = 0;
        bool loop = true;
        do
        {
            daeElementRef parent = node->getParent();
            if (parent && parent->typeID() == COLLADA_TYPE::NODE)
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

domNode* getRootJointNode(const domSkin* skin)
{
    std::vector<std::string> names;
    getJointNames(skin, names);
    daeSIDResolver resolver(const_cast<domSkin*>(skin)->getDocument()->getDomRoot(), names[0].c_str());
    daeElement* element = resolver.getElement();
    if (element && element->typeID() == COLLADA_TYPE::NODE)
    {
        domNode* node = daeSafeCast<domNode>(resolver.getElement());
        return node;
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
            if (element && element->typeID() == COLLADA_TYPE::SOURCE)
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
        if (element->typeID() == COLLADA_TYPE::NODE)
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

void getAnimationChannels(const domAnimationRef& animationRef, const std::string& nodeIdSlash, std::list<domChannelRef>& channels)
{
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

    // This animation could have child animations.
    const domAnimation_Array& animationArray = animationRef->getAnimation_array();
    unsigned int animationCount = animationArray.getCount();
    for (size_t i = 0; i < animationCount; ++i)
    {
        getAnimationChannels(animationArray[i], nodeIdSlash, channels);
    }
}

domVisual_scene* getVisualScene(const domCOLLADA::domSceneRef& domScene)
{
    daeElement* scene = domScene->getInstance_visual_scene()->getUrl().getElement();
    if (scene->typeID() == COLLADA_TYPE::VISUAL_SCENE)
    {
        return static_cast<domVisual_scene*>(scene);
    }

    // DAE_FBX sometimes doesn't export an ID. In that case, see if there is only one visual scene and use that.
    // Most of the time there is only one visual scene.
    domCOLLADA* root = (domCOLLADA*)domScene->getDocument()->getDomRoot();
    domLibrary_visual_scenes_Array& visualSceneLibrary = root->getLibrary_visual_scenes_array();
    size_t visualSceneLibraryCount = visualSceneLibrary.getCount();
    for (size_t i = 0; i < visualSceneLibraryCount; ++i)
    {
        domLibrary_visual_scenesRef scenesRef = visualSceneLibrary.get(i);
        domVisual_scene_Array visualScenes = scenesRef->getVisual_scene_array();
        size_t visualSceneCount = visualScenes.getCount();
        for (size_t j = 0; j < visualSceneCount; ++j)
        {
            domVisual_sceneRef visualScene = visualScenes.get(j);
            if (domVisual_scene* v = visualScene.cast())
            {
                return v;
            }
        }
    }
    return NULL;
}

domNode* getParent(domNodeRef node)
{
    daeElement* parent = node->getParent();
    if (parent && parent->typeID() == COLLADA_TYPE::NODE)
    {
        domNodeRef parentNode = daeSafeCast<domNode>(parent);
        return parentNode.cast();
    }
    return NULL;
}

domAnimation* getAnimation(domChannelRef channel)
{
    daeElement* parent = channel->getParent();
    if (parent && parent->typeID() == COLLADA_TYPE::ANIMATION)
    {
        domAnimationRef parentNode = daeSafeCast<domAnimation>(parent);
        return parentNode.cast();
    }
    return NULL;
}

domNode* getCommonNodeAncestor(std::list<domNodeRef>& nodes)
{
    if (nodes.empty())
        return NULL;
    if (nodes.size() == 1)
        return nodes.begin()->cast();

    std::list<domNode*> ancestors;
    size_t minAncestorCount = INT_MAX;
    for (std::list<domNodeRef>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        domNodeRef& node = *it;
        getNodeAncestors(node, ancestors);
        ancestors.push_back(node.cast());
        minAncestorCount = std::min(minAncestorCount, ancestors.size());
    }
    ancestors.resize(minAncestorCount);

    return ancestors.back();
}

void getNodeAncestors(domNodeRef& node, std::list<domNode*>& ancestors)
{
    ancestors.clear();
    domNode* parent = getParent(node);
    while (parent != NULL)
    {
        ancestors.push_front(parent);
        parent = getParent(parent);
    }
}

bool findGroupAnimationNodes(domCOLLADA* dom, std::vector<std::string>& nodesToGroup)
{
    bool groupPossible = false;
    const domLibrary_controllers_Array& controllersArrays = dom->getLibrary_controllers_array();
    size_t controllersArraysCount = controllersArrays.getCount();
    for (size_t i = 0; i < controllersArraysCount; ++i)
    {
        const domLibrary_controllersRef& libraryController = controllersArrays.get(i);
        const domController_Array& controllerArray = libraryController->getController_array();
        size_t controllerCount = controllerArray.getCount();
        for (size_t j = 0; j < controllerCount; ++j)
        {
            const domControllerRef& controllerRef = controllerArray.get(j);
            const domSkinRef& skinRef = controllerRef->getSkin();
            if (skinRef.cast() != NULL)
            {
                domSkin::domJointsRef joints = skinRef->getJoints();
                domInputLocal_Array& jointInputs = joints->getInput_array();
                for (unsigned int i = 0; i < jointInputs.getCount(); ++i)
                {
                    domInputLocalRef input = jointInputs.get(i);
                    std::string inputSemantic = std::string(input->getSemantic());
                    domURIFragmentType* sourceURI = &input->getSource();
                    sourceURI->resolveElement();
                    const domSourceRef source = (domSource*)(daeElement*)sourceURI->getElement();
                    if (equals(inputSemantic, "JOINT"))
                    {
                        std::list<domChannelRef> channels;
                        std::list<domNodeRef> nodes;
                        findChannelsTargetingJoints(source, channels, nodes);
                        // If the channels don't share the same animation then they can be grouped.
                        if (!sameAnimation(channels))
                        {
                            groupPossible = true;
                            domNode* parentMost = getCommonNodeAncestor(nodes);
                            nodesToGroup.push_back(parentMost->getId());
                        }
                    }
                }
            }
        }
    }
    return groupPossible;
}

bool sameAnimation(std::list<domChannelRef>& channels)
{
    std::list<domChannelRef>::iterator it = channels.begin();
    domAnimation* temp = getAnimation(*it);
    ++it;
    for (; it != channels.end(); ++it)
    {
        if (getAnimation(*it) != temp)
            return false;
    }
    return true;
}

void findChannelsTargetingJoints(const domSourceRef& source, std::list<domChannelRef>& channels, std::list<domNodeRef>& nodes)
{
    std::vector<std::string> jointNames;
    getJointNames(source, jointNames);
    for (std::vector<std::string>::iterator i = jointNames.begin(); i != jointNames.end(); ++i)
    {
        daeSIDResolver resolver(source->getDocument()->getDomRoot(), i->c_str());
        daeElement* element = resolver.getElement();
        if (element && element->typeID() == COLLADA_TYPE::NODE)
        {
            domNodeRef node = daeSafeCast<domNode>(element);
            nodes.push_back(node);
            getAnimationChannels(node, channels);
        }
    }
}

}
