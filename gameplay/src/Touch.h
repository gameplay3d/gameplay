#ifndef TOUCH_H_
#define TOUCH_H_

namespace gameplay
{

/**
 * Defines a touch screen event.
 */
class Touch
{
public:

    /**
     * Maximum simultaneous touch points supported.
     */
    static const unsigned int MAX_TOUCH_POINTS = 10;

    /**
     * The touch event type.
     */
    enum TouchEvent
    {
        TOUCH_PRESS,
        TOUCH_RELEASE,
        TOUCH_MOVE
    };

private:

    /**
     * Constructor. Used internally.
     */
    Touch();
};

}

#endif
