#ifndef GESTURE_H_
#define GESTURE_H_

namespace gameplay
{

/**
 * Gesture event
 */
class Gesture
{
public:

    enum GestureEvent
    {
        SWIPE   = 0,
        PINCH,
        ROTATE,
        TAP,
        TAP_DOUBLE,
        NONE = -1
    };

    static const int SWIPE_DIRECTION_UP = 1 << 0;
    static const int SWIPE_DIRECTION_DOWN = 1 << 1;
    static const int SWIPE_DIRECTION_LEFT = 1 << 2;
    static const int SWIPE_DIRECTION_RIGHT = 1 << 3;

private:

    /**
     * Constructor. Used internally.
     */
    Gesture();
};

}

#endif
