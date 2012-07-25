#ifndef MOUSE_H_
#define MOUSE_H_

namespace gameplay
{

/**
 * Mouse event
 */
class Mouse
{
public:

    /**
     * The mouse event type.
     */
    enum MouseEvent
    {
        MOUSE_PRESS_LEFT_BUTTON = 3,
        MOUSE_RELEASE_LEFT_BUTTON = 4,
        MOUSE_PRESS_MIDDLE_BUTTON = 5,
        MOUSE_RELEASE_MIDDLE_BUTTON = 6,
        MOUSE_PRESS_RIGHT_BUTTON = 7,
        MOUSE_RELEASE_RIGHT_BUTTON = 8,
        MOUSE_MOVE = 9,
        MOUSE_WHEEL = 10
    };


private:

    /**
     * Constructor. Used internally.
     */
    Mouse();
};

}

#endif
