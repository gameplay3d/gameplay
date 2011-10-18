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

    void add(AnimationChannel* animationChannel);
    /**
     * Returns the number of animation channels contained in this animation.
     * 
     * @return The number of animation channels.
     */
    unsigned int getAnimationChannelCount() const;

private:
    std::vector<AnimationChannel*> channels;
};

}
#endif
