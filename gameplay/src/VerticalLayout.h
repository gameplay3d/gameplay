#ifndef VERTICALLAYOUT_H_
#define VERTICALLAYOUT_H_

#include "Layout.h"
#include "Container.h"

namespace gameplay
{

/**
 * Defines a vertical layout.
 *
 * Controls are placed next to one another vertically until
 * the bottom-most edge of the container is reached.
 */
class VerticalLayout : public Layout
{
    friend class Form;
    friend class Container;

public:

    /**
     * Set whether this layout will start laying out controls from the bottom of the container.
     * This setting defaults to 'false', meaning controls will start at the top.
     *
     * @param bottomToTop Whether to start laying out controls from the bottom of the container.
     */
    void setBottomToTop(bool bottomToTop);

    /**
     * Get whether this layout will start laying out controls from the bottom of the container.
     *
     * @return Whether to start laying out controls from the bottom of the container.
     */
    bool getBottomToTop();

    /**
     * Get the type of this Layout.
     *
     * @return Layout::LAYOUT_VERTICAL
     */
    Layout::Type getType();

    /**
     * Returns the vertical spacing between controls in the layout.
     *
     * @return The vertical spacing between controls.
     */
    int getSpacing() const;

    /**
     * Sets the vertical spacing to add between controls in the layout.
     *
     * @param spacing The vertical spacing between controls.
     */
    void setSpacing(int spacing);

protected:

    /**
     * Constructor.
     */
    VerticalLayout();

    /**
     * Destructor.
     */
    virtual ~VerticalLayout();

    /**
     * Update the controls contained by the specified container.
     *
     * Controls are placed next to one another vertically until
     * the bottom-most edge of the container is reached.
     *
     * @param container The container to update.
     */
    void update(const Container* container);

    /**
     * Flag determining whether this layout will start laying out controls from the bottom of the container.
     * The default is 'false' meaning controls will start at the top.
     */
    bool _bottomToTop;

    /**
     * Spacing between controls in the layout.
     */
    int _spacing;

private:

    /**
     * Constructor.
     */
    VerticalLayout(const VerticalLayout& copy);

    /**
     * Create a VerticalLayout.
     *
     * @return a VerticalLayout object.
     */
    static VerticalLayout* create();

};

}

#endif