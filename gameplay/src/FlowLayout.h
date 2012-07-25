#ifndef FLOWLAYOUT_H_
#define FLOWLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

/**
 * Defines a layout that arranges controls in order, left-to-right, row by row.
 */
class FlowLayout : public Layout
{
    friend class Form;
    friend class Container;

public:

    /**
     * Get the type of this Layout.
     *
     * @return Layout::LAYOUT_FLOW
     */
    Layout::Type getType();

protected:

    /**
     * Update the controls contained by the specified container.
     *
     * @param container The container to update.
     * @param offset The offset position.
     */
    void update(const Container* container, const Vector2& offset);

private:

    /**
     * Constructor.
     */
    FlowLayout();

    /**
     * Constructor.
     */
    FlowLayout(const FlowLayout& copy);

    /**
     * Destructor.
     */
    virtual ~FlowLayout();

    /**
     * Create a FlowLayout.
     *
     * @return A FlowLayout object.
     */
    static FlowLayout* create();

};

}

#endif