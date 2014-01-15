#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "Ref.h"
#include "Touch.h"
#include "Vector2.h"

namespace gameplay
{

class Container;
class Control;

/**
 * Defines the layout for containers.
 *
 * Implementations are responsible for positioning, resizing and
 * calling update on all the controls within a container.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class Layout : public Ref
{
    friend class Container;
    friend class Form;

public:

    /**
     * Layout types available to containers.
     */
    enum Type
    {
        /**
         * Flow layout: Controls are placed next to one another horizontally
         * until the right-most edge of the container is reached, at which point
         * a new row is started.
         */
        LAYOUT_FLOW,

        /**
         * Vertical layout: Controls are placed next to one another vertically until
         * the bottom-most edge of the container is reached.
         */
        LAYOUT_VERTICAL,

        /**
         * Absolute layout: Controls are not modified at all by this layout.
         * They must be positioned and sized manually.
         */
        LAYOUT_ABSOLUTE
    };

    /**
     * Get the type of this layout.
     *
     * @return The type of this layout.
     */
    virtual Type getType() = 0;

protected:

    /**
     * Position, resize, and update the controls within a container.
     *
     * @param container The container to update.
     */
    virtual void update(const Container* container) = 0;

    /**
     * Touch callback on touch events.  Coordinates are given relative to the container's
     * content area.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @see Touch::TouchEvent
     */
    virtual bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

};

}

#endif