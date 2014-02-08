#ifndef FLOWLAYOUT_H_
#define FLOWLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

/**
 * Defines a flow layout.
 *
 * A flow layout arranges controls in order, left-to-right, row by row and wraps.
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

    /**
     * Returns the horizontal spacing between controls in the layout.
     *
     * @return The horizontal spacing between controls.
     */
    int getHorizontalSpacing() const;

    /**
     * Returns the vertical spacing between controls in the layout.
     *
     * @return The vertical spacing between controls.
     */
    int getVerticalSpacing() const;

    /**
     * Sets the spacing to add between controls in the layout.
     *
     * @param horizontalSpacing The horizontal spacing between controls.
     * @param verticalSpacing The vertical spacing between controls.
     */
    void setSpacing(int horizontalSpacing, int verticalSpacing);

protected:

    /**
     * Update the controls contained by the specified container.
     *
     * @param container The container to update.
     */
    void update(const Container* container);

    /**
     * Horizontal spacing between controls.
     */
    int _horizontalSpacing;

    /**
     * Vertical spacing between controls.
     */
    int _verticalSpacing;

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