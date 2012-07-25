#ifndef DAEUTIL_H_
#define DAEUTIL_H_

namespace gameplay
{

/**
 * Gets all of the animation channels that target the given node and appends them to the list.
 * 
 * @param node The node that the animation channels target.
 * @param channels The list of channels to append to.
 */
void getAnimationChannels(const domNodeRef& node, std::list<domChannelRef>& channels);

/**
 * Gets the joint names for the given source and appends them to the given list.
 * 
 * @param source The source element to search in.
 * @param list The list to append the joint names to.
 */
void getJointNames(const domSourceRef source, std::vector<std::string>& list);

/**
 * Gets the joint names for the given skin and appends them to the given list.
 * 
 * @param skin The skin element to search in.
 * @param list The list to append the joint names to.
 */
void getJointNames(const domSkin* skin, std::vector<std::string>& list);

/**
 * Gets the input source from the given channel.
 * 
 * @param channel The channel to search in.
 * 
 * @return The source element or NULL if not found.
 */
domSource* getInputSource(const domChannelRef& channel);

/**
 * Returns the sampler from the given channel.
 * 
 * @param channel The channel dom element.
 * 
 * @return The sampler or NULL if not found.
 */
const domSamplerRef getSampler(const domChannelRef& channel);

/**
 * Returns the source from the given sampler input.
 * Searchs within the given animation.
 * 
 * @param inputLocal The input element within a sampler.
 * @param animation The animation to search within.
 * 
 * @return The source or NULL if not found.
 */
const domSourceRef getSource(const domInputLocalRef& inputLocal, const domAnimationRef& animation);

/**
 * Returns the name array from the given source.
 * 
 * @param source The source element.
 * 
 * @return The name array or NULL if not found.
 */
const domName_arrayRef getSourceNameArray(const domSourceRef& source);

/**
 * Returns one skeleton from the given instance controller.
 * The COLLADA spec says that instance_controller can have multiple skeletons but we only need one.
 * Maya sometimes exports multiple skeleton nodes so this function will try to find the correct one.
 * 
 * @param instanceController The instance_controller element.
 * 
 * @return The skeleton or NULL if not found.
 */
const domInstance_controller::domSkeletonRef getSkeleton(const domInstance_controllerRef& instanceController);

/**
 * Returns the root joint node of the given skin.
 * 
 * @param skin The COLLADA skin to get the root joint for.
 * 
 * @return The COLLADA node or NULL if not found.
 */
domNode* getRootJointNode(const domSkin* skin);

/**
 * Returns true if the two given animation channels have equal key time input source.
 * 
 * @param c1 Channel one to compare.
 * @param c2 Channel two to compare.
 * 
 * @return True if the channels have the same key times, false otherwise.
 */
bool equalKeyTimes(const domChannelRef& c1, const domChannelRef& c2);

/**
 * Returns true if the two sources have the same key times.
 * 
 * @param s1 Source one.
 * @param s2 Source two.
 * 
 * @return True true if the key times are equal, false otherwise.
 */
bool equalKeyTimes(const domSource* s1, const domSource* s2);

/**
 * Moves a channel and the sources it uses to the destination animation.
 * 
 * @param channel The channel to move.
 * @param animation The destination animation to copy to.
 */
void moveChannelAndSouresToAnimation(domChannelRef& channel, domAnimationRef& animation);

/**
 * Returns true if the given animation is empty and contains no children.
 * 
 * @param animation The animation element to check.
 * 
 * @return True if the animation has no children, false otherwise.
 */
bool isEmptyAnimation(domAnimationRef& animation);

/**
 * Gets the visual scene from the given COLLADA dom scene.
 * 
 * @param domScene The dom scene.
 * 
 * @return The visual scene or NULL if not found.
 */
domVisual_scene* getVisualScene(const domCOLLADA::domSceneRef& domScene);

/**
 * Returns the parent node of the given node or NULL if there is no parent.
 * 
 * @param node The node to get the parent for.
 * 
 * @return The parent node or NULL if the node does not have a parent node.
 */
domNode* getParent(domNodeRef node);

/**
 * Returns the animation for the given channel.
 * 
 * @param channel The animation channel to get the animation for.
 * 
 * @return The animation of the channel or NULL if the channel does not belong to an animation.
 */
domAnimation* getAnimation(domChannelRef channel);

/**
 * Gets the common node ancestor for the given list of nodes.
 * This function assumes that the nodes share a common ancestor.
 * 
 * @param nodes The list of nodes.
 * 
 * @return The common node ancestor or NULL if the list of was empty.
 */
domNode* getCommonNodeAncestor(std::list<domNodeRef>& nodes);

/**
 * Gets the list of node ancestors for the given node.
 * 
 * @param node The node to get the ancestors for.
 * @param ancestors The output list of ancestors. 
 *                  The first element is the root node and the last element is the direct parent of the node.
 */
void getNodeAncestors(domNodeRef& node, std::list<domNode*>& ancestors);

/**
 * Finds the nodes that can be automatically grouped because there is a mesh skin that has joints 
 * that are being targetted by animations that are not grouped.
 * 
 * @param dom The COLLADA dom.
 * @param nodesToGroup The list of node IDs that can have their animations automatically grouped under.
 * 
 * @return True if there are mesh skins that can have their animations grouped, false otherwise.
 */
bool findGroupAnimationNodes(domCOLLADA* dom, std::vector<std::string>& nodesToGroup);

/**
 * Returns true if the list of animation channels share the same animation.
 * 
 * @param channels The list of channels.
 * 
 * @return True if the channels share the same animation, false otherwis.
 */ 
bool sameAnimation(std::list<domChannelRef>& channels);

/**
 * Finds the animation channels that target the given joints and the list of nodes that are targetted by those channels.
 * 
 * @param source The source element to get the list of joints from.
 * @param channels The output list of channels.
 * @param nodes The output list of nodes.
 */
void findChannelsTargetingJoints(const domSourceRef& source, std::list<domChannelRef>& channels, std::list<domNodeRef>& nodes);

}

#endif
