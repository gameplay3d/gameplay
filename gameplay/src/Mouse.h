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
        MOUSE_LEFT_BUTTON_PRESS,
        MOUSE_LEFT_BUTTON_RELEASE,
        MOUSE_MIDDLE_BUTTON_PRESS,
        MOUSE_MIDDLE_BUTTON_RELEASE,
        MOUSE_RIGHT_BUTTON_PRESS,
        MOUSE_RIGHT_BUTTON_RELEASE,
        MOUSE_MOVE
    };


private:

    /**
     * Constructor. Used internally.
     */
    Mouse();
};

}

#endif
