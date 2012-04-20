#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Object.h"
#include "AnimationChannel.h"

namespace gameplay
{

class Animation : public Object
{
public:

    /**
     * Constructor.
     */
    Animation(void);

    /**
     * Destructor.
     */
    virtual ~Animation(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    /**
     * Adds the given animation channel to this animation.
     * 
     * @param animationChannel The animation channel to add.
     */
    void add(AnimationChannel* animationChannel);

    /**
     * Removes the given animation channel from this animation.
     * 
     * @param animationChannel The animation channel to remove.
     */
    void remove(AnimationChannel* animationChannel);

    /**
     * Returns the number of animation channels contained in this animation.
     * 
     * @return The number of animation channels.
     */
    unsigned int getAnimationChannelCount() const;

    /**
     * Returns the animation channel at the given index.
     * 
     * @param index The index of the animation channel to get.
     * 
     * @return The pointer to the animation channel or NULL if not found.
     */
    AnimationChannel* getAnimationChannel(unsigned int index) const;

private:

    std::vector<AnimationChannel*> _channels;
};

}

#endif
