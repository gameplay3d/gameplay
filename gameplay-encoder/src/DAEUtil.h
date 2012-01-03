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

}

#endif
