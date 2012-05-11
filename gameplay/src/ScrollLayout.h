#ifndef SCROLLLAYOUT_H_
#define SCROLLLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

class ScrollLayout : public Layout
{
    friend class Form;
    friend class Container;

public:

    /**
     * Get the type of this Layout.
     *
     * @return Layout::LAYOUT_SCROLL
     */
    Layout::Type getType();

protected:

    /**
     * Create a ScrollLayout.
     *
     * @return A ScrollLayout object.
     */
    static ScrollLayout* create();

    /**
     * Update the controls contained by the specified container.
     *
     * @param container The container to update.
     */
    void update(const Container* container);

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

private:

    /**
     * Constructor.
     */
    ScrollLayout();

    /**
     * Constructor.
     */
    ScrollLayout(const ScrollLayout& copy);

    /**
     * Destructor.
     */
    virtual ~ScrollLayout();

    Vector2 _scrollPosition;

    // Previous touch point.
    int _lastX;
    int _lastY;

    bool _scrolling;

    bool _positionVertically;
    bool _positionHorizontally;
};

}

#endif