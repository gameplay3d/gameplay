#include "Joystick.h"

namespace gameplay
{

inline const Vector2& Joystick::getValue() const
{
    return _value;
}

inline void Joystick::setInnerRegionSize(const Vector2& size)
{
    if (_innerSize)
        _innerSize->set(size);
}

inline const Vector2& Joystick::getInnerRegionSize() const
{
    if (_innerSize)
        return *_innerSize;
    else
        return Vector2::zero();
}

inline void Joystick::setOuterRegionSize(const Vector2& size)
{
    if (_outerSize)
        _outerSize->set(size);
}

inline const Vector2& Joystick::getOuterRegionSize() const
{
    if (_outerSize)
        return *_outerSize;
    else
        return Vector2::zero();
}

inline void Joystick::setRelative(bool relative)
{
    _relative = relative;
}

inline bool Joystick::isRelative() const
{
    return _relative;
}

inline const unsigned int Joystick::getIndex() const
{
    return _index;
}

}
