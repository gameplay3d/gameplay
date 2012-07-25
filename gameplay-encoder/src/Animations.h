#ifndef LIBRARYANIMATIONS_H_
#define LIBRARYANIMATIONS_H_

#include "Object.h"
#include "Animation.h"

namespace gameplay
{

/**
 * Animations contains all of the animations in the GamePlay Binary file.
 */
class Animations : public Object
{
public:

    /**
     * Constructor.
     */
    Animations(void);

    /**
     * Destructor.
     */
    virtual ~Animations(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    void add(Animation* animation);
    unsigned int getAnimationCount() const;
    Animation* getAnimation(unsigned int index) const;
    void removeAnimation(unsigned int index);

private:

    std::vector<Animation*> _animations;
};

}

#endif
