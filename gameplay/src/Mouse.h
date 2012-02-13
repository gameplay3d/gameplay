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
        MOUSE_PRESS_LEFT_BUTTON,
        MOUSE_RELEASE_LEFT_BUTTON,
        MOUSE_PRESS_MIDDLE_BUTTON,
        MOUSE_RELEASE_MIDDLE_BUTTON,
        MOUSE_PRESS_RIGHT_BUTTON,
        MOUSE_RELEASE_RIGHT_BUTTON,
        MOUSE_MOVE,
        MOUSE_WHEEL
    };


private:

    /**
     * Constructor. Used internally.
     */
    Mouse();
};

}

#endif
