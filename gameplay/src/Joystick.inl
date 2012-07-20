#include "Joystick.h"

namespace gameplay
{

inline const Vector2& Joystick::getValue() const
{
    return _value;
}

inline void Joystick::setInnerSize(const Vector2& size)
{
    if (_innerSize)
        _innerSize->set(size);
}

inline const Vector2& Joystick::getInnerSize() const
{
    Vector2 size(0, 0);
    if (_innerSize)
        size.set(*_innerSize);
    
    return size;
}

inline void Joystick::setOuterSize(const Vector2& size)
{
    if (_outerSize)
        _outerSize->set(size);
}

inline const Vector2& Joystick::getOuterSize() const
{
    Vector2 size(0, 0);
    if (_outerSize)
        size.set(*_outerSize);
    
    return size;
}

inline void Joystick::setRelative(bool relative)
{
    _relative = relative;
}

inline bool Joystick::isRelative() const
{
    return _relative;
}

}
