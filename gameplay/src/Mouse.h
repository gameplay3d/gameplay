#ifndef MOUSE_H_
#define MOUSE_H_

namespace gameplay
{

/**
 * Defines a mouse event.
 */
class Mouse
{
public:

    /**
     * The mouse event type.
     */
    enum MouseEvent
    {
        MOUSE_PRESS_LEFT_BUTTON,
        MOUSE_RELEASE_LEFT_BUTTON, 
        MOUSE_PRESS_MIDDLE_BUTTON = 3,
        MOUSE_RELEASE_MIDDLE_BUTTON = 4,
        MOUSE_PRESS_RIGHT_BUTTON = 5,
        MOUSE_RELEASE_RIGHT_BUTTON = 6,
        MOUSE_MOVE = 7,
        MOUSE_WHEEL = 8
    };


private:

    /**
     * Constructor. Used internally.
     */
    Mouse();
};

}

#endif
