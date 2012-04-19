#ifndef FLOWLAYOUT_H_
#define FLOWLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

/**
 *  Defines a layout that arranges components in a left-to-right or right-to-left flow. 
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
     * Create a FlowLayout.
     *
     * @return A FlowLayout object.
     */
    static FlowLayout* create();

    /**
     * Update the controls contained by the specified container.
     *
     * @param container The container to update.
     */
    void update(const Container* container);

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

};

}

#endif