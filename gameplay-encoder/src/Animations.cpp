#include "Base.h"
#include "Animations.h"

namespace gameplay
{

Animations::Animations(void)
{
    // There will only be one Animations.
    // It requires an ID because it will be stores in the ref table.
    setId("__Animations__");
}

Animations::~Animations(void)
{
}

unsigned int Animations::getTypeId(void) const
{
    return ANIMATIONS_ID;
}
const char* Animations::getElementName(void) const
{
    return "Animations";
}

void Animations::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write((unsigned int)_animations.size(), file);
    for (std::vector<Animation*>::iterator i = _animations.begin(); i != _animations.end(); ++i)
    {
        (*i)->writeBinary(file);
    }
}

void Animations::writeText(FILE* file)
{
    fprintElementStart(file);
    if (_animations.size() > 0 )
    {
        for (std::vector<Animation*>::iterator i = _animations.begin(); i != _animations.end(); ++i)
        {
            (*i)->writeText(file);
        }
    }
    fprintElementEnd(file);
}

void Animations::add(Animation* animation)
{
    _animations.push_back(animation);
}

unsigned int Animations::getAnimationCount() const
{
    return _animations.size();
}

Animation* Animations::getAnimation(unsigned int index) const
{
    return _animations[index];
}

void Animations::removeAnimation(unsigned int index)
{
    _animations.erase(_animations.begin() + index);
}

}
