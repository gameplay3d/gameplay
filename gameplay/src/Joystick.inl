#include "Joystick.h"

namespace gameplay
{

inline const Vector2& Joystick::getValue() const
{
    return _value;
}

inline void Joystick::setRegion(const Rectangle& region)
{
    if (_region.isEmpty())
        _region = _bounds;

    _bounds = region;
}

inline const Rectangle& Joystick::getRegion() const
{
    return _bounds;
}

inline void Joystick::setAbsolute(bool absolute)
{
    _absolute = absolute;
}

inline bool Joystick::isAbsolute() const
{
    return _absolute;
}

}
