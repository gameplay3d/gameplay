#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "Ref.h"

namespace gameplay
{

class Container;

/**
 * The layout interface for UI containers.
 *
 * Implementations of this interface are responsible for positioning, resizing
 * and then calling update on all the controls within a container.
 */
class Layout : public Ref
{
    friend class Container;

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
};

}

#endif